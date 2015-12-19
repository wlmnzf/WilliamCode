#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#define NTEXT 10
#define NTCB 10
#define NBUF 10
struct TCB;
struct buffer;


typedef struct
{
   int value;
   struct TCB *wq;
}semaphore;
struct buffer
{
   int sender;
   int size;
   char text [NTEXT];
   struct buffer *next;
};
struct TCB
{
   unsigned char* stack;
   unsigned ss;
   unsigned sp;
   char state;
   char name[10];
   struct buffer *mp;
   semaphore mutex;
   semaphore sm;
}tcb[NTCB];



struct buffer *freebuf;
struct buffer **mq;

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
      if(buff=NULL) return;
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
      struct buffer *temp;
      if(*mq==NULL)  return;
      else
       {
		  temp=*mq;
	      while(temp->next!=NULL)
	       {
			 if(temp->sender==sender)
			  {
			     return temp;
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
      /*获取空闲消息缓冲区*/
          p(&sfb);
          p(&mutexfb);
            buff=getbuf();
          v(&mutexfb);
         /*填写消息缓冲区各项信息*/
          buff->id=current;
          buff->size=size;
          buff->next=NULL;
          for(i=0;i<buff->size;i++,a++)
            buff->text[i]=*a;
         /*将消息缓冲区插入到接收者线程的消息队列末尾*/
          p(&tcb[id].mutex);
            insert(&(tcb[id].mq),buff);
          v(&tcb[id].mutex);
          v(&tcb[id].sm);
        
          enable();

}
void receive()
{

}

int main()
{
    freebuf= (struct buffer*)malloc(sizeof(struct buffer));
    int i=0;
    struct buffer *p=freebuf;
    for(;i<NBUF-1;i++)
    {
       struct buffer *node=(struct buffer*)malloc(sizeof(struct buffer));
       p->next=node;
    }
    p->next=NULL;



}