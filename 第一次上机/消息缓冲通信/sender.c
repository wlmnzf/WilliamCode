#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#define MSG_FILE "sender.c"
#define BUFFER 255
#define PERM S_IRUSR|S_IWUSR
struct msgbuf
{
    long mtype;
    char mtext[BUFFER+1];
};

int main()
{
    struct msgbuf msg;
    key_t key;
    int msgid;
    int i;
    pid_t pid;
   char message[BUFFER];

    if((key=ftok(MSG_FILE,66))==-1)
    {
        fprintf(stderr,"Creat Key Error：%s \n",strerror(errno));
        exit(EXIT_FAILURE);
    }


    pid=fork();
    if(pid<0)
    {
        printf("fork error\n");
        exit(-1);
    }

    if(pid>0)
    {
	    if((msgid=msgget(key,PERM|IPC_CREAT))==-1)
	    {
		fprintf(stderr,"Creat MessageQueue Error：%s \n",strerror(errno));
		exit(EXIT_FAILURE);
	    }
	    msg.mtype=1;
	    while(1)
	    {
	    	printf("please input something!\n");
	    	scanf("%s",message);
	    	if(strcmp(message,"exit")==0) 
	    	{
	    		strcpy(message,"end");
	    	}
		strncpy(msg.mtext,message,BUFFER);
		msgsnd(msgid,&msg,sizeof(struct msgbuf),0);
		if(strcmp(message,"end")==0)  break;
	    }
	    memset(&msg,'\0',sizeof(struct msgbuf));
	    msgrcv(msgid,&msg,sizeof(struct msgbuf),2,0);
	    printf("Sender receive: %s\n",msg.mtext);
	    if(msgctl(msgid, IPC_RMID, 0) == -1)
	    {
		fprintf(stderr, "Remove MessageQueue Error%s \n",strerror(errno));
		exit(EXIT_FAILURE);
	    }
	    exit(EXIT_SUCCESS);
    }
    else
    {
	    char *myask="over\n";
	    if((key=ftok(MSG_FILE,66))==-1)
	    {
		fprintf(stderr,"Creat Key Error：%s \n",strerror(errno));
		exit(EXIT_FAILURE);
	    }
	    if((msgid=msgget(key,PERM|IPC_CREAT))==-1)
	    {
		fprintf(stderr,"Creat MessageQueue Error：%s \n",strerror(errno));
		exit(EXIT_FAILURE);
	    }
	    while(strcmp(msg.mtext,"end")!=0)
	    {
		msgrcv(msgid,&msg,sizeof(struct msgbuf),1,0);
		printf("Receiver receive: %s\n",msg.mtext);
	    }
	    msg.mtype=2;
	    strncpy(msg.mtext,myask,BUFFER);
	    msgsnd(msgid,&msg,sizeof(struct msgbuf),0);

	    exit(EXIT_SUCCESS);
    }




}
