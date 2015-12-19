#include<stdio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>

#define NTEXT 10
#define NTCB 10
#define NBUF 10
#define FINISHED 0
#define RUNNING 1
#define READY 2
#define BLOCKED 3
#define GET_INDOS 0x34
#define GET_CRIT_ERR 0x5d06

void over(void);
void InitDos(void);
int DosBusy(void);
void interrupt new_int8();
void interrupt(*old_int8)();
void interrupt my_swtch();
void block(struct TCB **qp);
void wakeup_first(struct TCB **qp);
int find();


struct TCB;
struct buffer;




typedef struct{
  int value;
  struct TCB *wq;
}semaphore;



struct buffer{
  int sender;
  int size;
  char text[NTEXT];
  struct buffer* next;
}*freebuf;


struct TCB{
  unsigned char* stack;
  unsigned ss;
  unsigned sp;
  char state;
  char name[10];
  struct TCB* next;
  struct buffer *mq;
  semaphore mutex;
  semaphore sm;

}tcb[NTCB];


struct int_regs{
   unsigned bp,di,si,ds,es,dx,cx,bx,ax,ip,cs,flags,off,seg;

};




char far *indos_ptr=0;
char far *crit_err_ptr=0;
int TL = 1;
int current = 0;
int timecount = 0;

struct buffer **mq;
int buff[5];
semaphore sfb;
semaphore mutexfb;
semaphore emptymutex;
semaphore fullmutex;



// wgh  initTCB()  create()  destory() over()

void initTCB(void){
   int id;
   for(id=0;id<NTCB;id++){
		tcb[id].stack = NULL;
		tcb[id].state = FINISHED;
		tcb[id].name[0] = '\0';
		tcb[id].next = NULL;
		tcb[id].mq = NULL;
		tcb[id].mutex.value = 1;
		tcb[id].mutex.wq = NULL;
		tcb[id].sm.value = 0;
		tcb[id].sm.wq = NULL;
   }
}


typedef int (far* codeptr)(void);


int create(char* name,codeptr code,int stck){
  struct int_regs far* regs;
  int id;
  disable();
  for(id=1;id<NTCB;id++){
    if(tcb[id].state==FINISHED)
      break;
  }
  if(id==NTCB){
    printf("Thread %s create falied",name);
    return -1;
  }
  tcb[id].stack=(unsigned char*)malloc(stck);
  regs=(struct int_regs far*)(tcb[id].stack+stck);
  regs--;


  regs->ds=_DS;
  regs->es=_ES;
  regs->ip=FP_OFF(code);
  regs->cs=FP_SEG(code);
  regs->flags=0x200;
  regs->off=FP_OFF(over);
  regs->seg=FP_SEG(over);

  strcpy(tcb[id].name,name);
  tcb[id].ss=FP_SEG(regs);
  tcb[id].sp=FP_OFF(regs);
  tcb[id].state=READY;
  printf("\n thread %d %s has already cteated\n",id,tcb[id].name);
  enable();
  return id;
}

void destory(int id){
	disable();
	free(tcb[id].stack);
	tcb[id].stack=NULL;
	tcb[id].state=FINISHED;
	tcb[id].name[0]='\0';
	tcb[id].next=NULL;
	tcb[id].mq=NULL;
	tcb[id].mutex.value=1;
	tcb[id].mutex.wq=NULL;
	tcb[id].sm.value=0;
	tcb[id].sm.wq=NULL;
	printf("\n thread %d %s has already destoryed \n",id,tcb[id].name);
	enable();
}


void over(void){
  destory(current);
  my_swtch();
}


//jwt   InitDos(),DosBusy(),new_int8(),my_swtch(),int find()

void InitDos()
{
union REGS regs;
struct SREGS segregs;
regs.h.ah=GET_INDOS;
intdosx(&regs,&regs,&segregs);
indos_ptr=(char far*)MK_FP(segregs.es,regs.x.bx);
if (_osmajor < 3)
	crit_err_ptr = indos_ptr + 1;
else if( _osmajor == 3 && _osminor == 0)
	crit_err_ptr = indos_ptr - 1;
else {
	regs.x.ax = GET_CRIT_ERR;
	intdosx(&regs, &regs, &segregs);
	crit_err_ptr =(char far*) MK_FP(segregs.ds, regs.x.si);
}
}
int DosBusy()
{
	if (indos_ptr&&crit_err_ptr)
		return(*indos_ptr || *crit_err_ptr);
	else
		return(-1);
}

void interrupt new_int8()
{
	(*old_int8)();
		timecount++;
	if (timecount < TL || DosBusy())
		return;
	my_swtch();
}

void interrupt my_swtch()
{
	int i;
	disable();
	tcb[current].ss = _SS;
	tcb[current].sp = _SP;
	if (tcb[current].state == RUNNING)
		tcb[current].state = READY;
	i = find();
	if (i < 0)
		i = 0;
	_SS = tcb[i].ss;
	_SP = tcb[i].sp;
	tcb[i].state = RUNNING;
	current = i;
	timecount = 0;
	enable();
}


int find()
{
	int id;
	for (id = current + 1; id < NTCB; id++)
	{
		if (tcb[id].state == READY)
			return id;
	}
	for (id = 0; id < current; id++)
	{
		if (tcb[id].state == READY)
			return id;
	}
	return -1;
}

//ljf p(),v(),block(),wakeup_first()

void p(semaphore *sem)
{
   struct TCB**qp;
   disable();
   sem->value=sem->value-1;
   if(sem->value<0){qp=&(sem->wq);block(qp);}
   enable();
}

void v(semaphore*sem)
{
  struct TCB**qp;
  disable();
  qp=&(sem->wq);
  sem->value=sem->value+1;
  if(sem->value<=0)wakeup_first(qp);
  enable();
}

void block(struct TCB **qp)
{
  struct TCB *tcbp;
  tcb[current].state=BLOCKED;
  tcb[current].next=NULL;
  if((*qp)==NULL){
  (*qp)=&tcb[current];}
  else{
       tcbp=*qp;
       while(tcbp->next!=NULL)
       tcbp=tcbp->next;
       tcbp->next=&tcb[current];
	}
  my_swtch();
}


void wakeup_first(struct TCB **qp)
{
    struct TCB *tcbp;
    if((*qp)==NULL) return;
    tcbp=*qp;
    *qp=(*qp)->next;
    tcbp->next=NULL;
    tcbp->state=READY;
}

void tcb_state()
{
    int id;
    printf("\n  **** The current threads' state ****\n");
    for(id = 0; id < NTCB; id++)
    {
    	printf("Thread %d %9s state is ", id, tcb[id].name);
    	switch(tcb[id].state)
    	{
    	    case FINISHED: puts("FINISHED"); break;
    	    case RUNNING: puts("RUNNING"); break;
    	    case READY: puts("READY"); break;
    	    case BLOCKED: puts("BLOCKED"); break;
    	}
    }
}
int finished()
{
    int id;
    for(id = 1; id < NTCB; id++)
    {
	     if(tcb[id].state != FINISHED)
	     return 0;
    }
    return 1;
}

//wlm show() showbuff() getbuff() insert() remov() send() receive()
int menu()
{
  int id;
  printf("/=================================/\n");
  printf("  1.FCFS Demo\n");
  printf("  2.Round-Robin Demo\n");
  printf("  3.Mutex Demo\n");
  printf("  4.Sync Demo\n");
  printf("  5.Buffer Demo\n");
  printf("  6.Quit\n");
  printf("  Please choose !\n");
  printf("/================================/\n");

  scanf("%d",&id);
  return id;
}

void show(id)
{
	struct buffer *buff=tcb[id].mq;
	printf("%d ",tcb[id].sm.value);
	while(buff!=NULL)
	{
		printf("%s ",buff->text);
		buff=buff->next;
	}
}

void showbuff(struct buffer *buff)
{
	if(buff==NULL)
		printf("NULL  %d\n",current);
	else
	{
		printf("id =>%d\n",buff->sender);
		printf("sender =>%d\n",buff->sender);
		printf("size =>%d\n",buff->size);
		printf("text =>%s\n",buff->text);
	}
}

struct buffer *getbuf(void)
{
   struct buffer *buff;
   buff=freebuf;
   freebuf=freebuf->next;
   return buff;
}
void insert(struct buffer **mq,struct buffer *buff)
{
      struct buffer *temp;
      if(buff==NULL) return;
      buff->next=NULL;
      if(*mq==NULL)
	      *mq=buff;
      else
       {
			 temp=*mq;
			 while(temp->next!=NULL)
			    temp=temp->next;
			 temp->next=buff;
       }
}

struct buffer *remov(struct buffer **mq,int sender)
{
      struct buffer *pre;
	  struct buffer *target;

      if(mq==NULL)  return NULL;
      else
       {
		  pre=target=*mq;
		  if(target->sender==sender)
		  {
			  *mq=target->next;
			  target->next=NULL;
			  return target;
		  }
		  target=target->next;
	      while(target!=NULL)
	       {
			  if(target->sender==sender)
			  {
				 pre->next=target->next;
				 target->next=NULL;
				 return target;
			  }
			  else
			  {
				  pre=target;
				  target=target->next;
			  }
	       }
       }
       return NULL;
}



void send(char *receiver,char *a,int size)
{
    struct buffer *buff;
    int i,id=-1;

    disable();
    for(i=0;i<NTCB;i++)
     {
		if(strcmp(receiver,tcb[i].name)==0)
		{
		   id=i;
		   break;
		}
     }
     if(id==-1)
     {
		 printf("Error:Receiver not exsist!\n");
		 enable();
		 return;
     }

	  p(&sfb);
	  p(&mutexfb);
	  buff=getbuf();
	  v(&mutexfb);
	  buff->sender=current;
	  buff->size=size;
	  buff->next=NULL;


	 strcpy(buff->text,a);

	  p(&tcb[id].mutex);
	  insert(&(tcb[id].mq),buff);
	  v(&tcb[id].mutex);
	  v(&(tcb[id].sm));

	  enable();

}
void receive(char *sender,char *b)
{
    struct buffer *buff;
    int i,id=-1;

    disable();

		 for(i=0;i<NTCB;i++)
		 {
			if(strcmp(sender,tcb[i].name)==0)
			{
			   id=i;
			   break;
			}
		 }
		 if(id==-1)
		 {
			 printf("Error:Sender not exsist!\n");
			 enable();
			 return;
		 }

    p(&tcb[current].sm);
	  p(&tcb[current].mutex);
	  buff=remov(&(tcb[current].mq),id);
	  v(&tcb[current].mutex);
	  strcpy(b,buff->text);


	  p(&mutexfb);
	    insert(&(freebuf),buff);
		v(&sfb);
	  v(&mutexfb);
	  enable();
}
