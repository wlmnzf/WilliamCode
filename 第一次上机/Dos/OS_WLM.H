void sender()
{
	char a[NTEXT]={'\0'};
	printf("please write something\n");
	scanf("%s",a);

	send("receiver",a,strlen(a));
	strcpy(a,"");
	receive("receiver",a);
	
	if(strlen(a)>0)
	  {
         if(strcmp(a,"get")==0)
		   {
			  printf("send successfully\n");
		   }
	  }
}

void receiver()
{
	char b[NTEXT]={'\0'};
	strcpy(b,"");
	receive("sender",b);
	if(strlen(b)>0)
	  {
          printf("sender say:%s\n",b);
          strcpy(b,"get");
          send("sender",b,strlen(b));
          sleep(2);
      }

}


int wlm_show_time_buffer()
{
     int i=0;
     struct buffer *p;
     mutexfb.value=1;
     sfb.value=NBUF;
     freebuf= (struct buffer*)malloc(sizeof(struct buffer));
     p=freebuf;

    for(;i<NBUF-1;i++)
    {
       struct buffer *node=(struct buffer*)malloc(sizeof(struct buffer));
       p->next=node;
       p=p->next;
    }
    p->next=NULL;

    InitDos();
    initTCB();
    old_int8=getvect(8);
          
    strcpy(tcb[0].name, "main");
    tcb[0].state=RUNNING;
    current=0;

    create("sender",(codeptr)sender,1024);
    create("receiver",(codeptr)receiver,1024);


    setvect(8,new_int8);
    my_swtch();

	while(!finished());

	tcb[0].name[0]='\0';
    tcb[0].state=FINISHED;
	setvect(8,old_int8);

	printf("\n Multi_task system terminated. \n");

    return 0;
}



void input()
{
   int i=1;
   int ans=0;
   int j=0;
   for(;i<=50;i++)
   {
      p(&emptymutex);
      ans=i*i;
      printf("count=>%d\n",i*i);
	   
	  for(j=0;j<5;j++)
	  {
	     if(buff[j]==-1)
	     {
	       buff[j]=ans;
		   v(&fullmutex);
	       break;
	     }

	  }
	  sleep(1);
   }

}
void output()
{
     int j=0;
	 int cnt=0;
     
     while(cnt<50)
	 {
		 p(&fullmutex);
		 for(j=0;j<5;j++)
		  {
			 if(buff[j]!=-1)
			   {
				  printf("read=>%d\n",buff[j]);
				  cnt++;
				  buff[j]=-1;
				  v(&emptymutex);
				  break;
			   }
		  }
		  sleep(1);
	 }
}
int wlm_show_time_sync()
{
    int i=0;
	emptymutex.value=5;
	fullmutex.value=0;

	for(;i<5;i++)
	    buff[i]=-1;
	
    InitDos();
    initTCB();
    old_int8=getvect(8);
          
    strcpy(tcb[0].name, "main");
    tcb[0].state=RUNNING;
    current=0;

    create("input",(codeptr)input,1024);
    create("output",(codeptr)output,1024);

    setvect(8,new_int8);
    my_swtch();

    while(!finished());

    tcb[0].name[0]='\0';
    tcb[0].state=FINISHED;
    setvect(8,old_int8);

    printf("\n Multi_task system terminated. \n");
   
    return 0;
}