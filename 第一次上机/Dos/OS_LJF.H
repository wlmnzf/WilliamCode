semaphore empty;
semaphore applefull;
semaphore orangefull;
int i,j;

void fa()
	  {     
	  i=3;
	     while(i){
		p(&empty);
		printf("father input an apple.\n");
		v(&applefull);
		 i=i-1;
	      }
         sleep(1);
	   }
void mom()
	  {     
	  j=4;
	     while(j){
		p(&empty);
		printf("mother input an orange.\n");
		v(&orangefull);
		j=j-1;
	     }
	     sleep(1);
	   }
void son()
	  {
	 while(j){
   p(&orangefull);
	    printf("son get an orange.\n");
	    v(&empty);

	    }
	    sleep(1);
	   }
void dau()
	  {
   while(i){
   p(&applefull);
	    printf("daughter get an apple.\n");
	    v(&empty);

	     }
        sleep(1);
	   }


int ljf_show_time()
{  
   InitDos();
   initTCB();

  old_int8 =getvect(8);

   strcpy(tcb[0].name,"main");
   tcb[0].state=RUNNING;
   current=0;

 create("fa",(codeptr)fa,1024);
 create("mom",(codeptr)mom,1024);
 create("son",(codeptr)son,1024);
 create("dau",(codeptr)dau,1024);
 empty.value=1;
 applefull.value=0;
 orangefull.value=0;

 setvect(8,new_int8);
 my_swtch();

 while(!finished());
 tcb[0].name[0]='\0';
 tcb[0].state=FINISHED;
 setvect(8,old_int8);
 printf("\nThanks for using multi_task system.\n");
return 0;
}