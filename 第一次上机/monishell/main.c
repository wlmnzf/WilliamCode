#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    pid_t id;
    char cmd[100]={'\0'};
    strcmp(cmd,"");
    
    while(1)
    {
       printf("please input some command:\n");
       scanf("%s",cmd);
       if(!(strcmp(cmd,"exit")))  return;
       char * argv[ ]={(char *) 0 };
       id =fork();
	    if(id==0)
	    {
                execv(cmd,argv);
                printf("Command is not found");  
	    }
            else
            {           
                pid_t pr = wait(NULL);
	    }
    }
}
