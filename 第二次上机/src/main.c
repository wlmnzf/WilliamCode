#include "fs.h"
#include "wlm.h"
#include "jwt.h"
#include "wgh.h"
#include "ljf.h"

char cmd_head[100]="root>";
char cmd[100];

void solve_cmd(char *cmd_text)
{
	char *cmd;
	char *param;
    cmd = strtok(cmd_text," ");
    param=strtok(NULL," ");
     
    if(!strcmp(cmd,"my_format"))
	    {
			  my_format();
			  my_exitsys();
			  startsys();
	    }
    else if(!strcmp(cmd,"my_mkdir"))
        {
        	 if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
              my_mkdir(param);
        }
    else if(!strcmp(cmd,"my_rmdir"))
        {
     	     if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
              my_rmdir(param);
        }
    else if(!strcmp(cmd,"my_ls"))
       {
              my_ls();
       }
    else if(!strcmp(cmd,"my_cd"))
       {
       	    if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
              my_cd(param);
	   }
    else if(!strcmp(cmd,"my_crete"))
       {
       	    if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
	          my_create(param);
       }
    else if(!strcmp(cmd,"my_open"))
      {
      	    if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
	          my_open(param);
	  }
    else if(!strcmp(cmd,"my_close"))
     {
     	   if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
			  int tmp_fd=find_fd_by_name(param);
		      if(tmp_fd==-1)
		         {
				     printf("���ļ�δ����!\n");
				 }
			  else
				 {
					 my_close(tmp_fd);
					 tmp_fd=-1;
				 }
	 }
    else if(!strcmp(cmd,"my_write"))
     {
 	     if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
          int tmp_fd=find_fd_by_name(param);
          if(tmp_fd==-1)
             {
			    printf("���ļ�δ����!\n");
			 }
	      else
			 {
				 my_write(tmp_fd);
				 tmp_fd=-1;
			 }
	 }
   else if(!strcmp(cmd,"my_read"))
     {
     	   if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
	       int tmp_fd=find_fd_by_name(param);
           if(tmp_fd==-1)
             {
			    printf("���ļ�δ����!\n");
			 }
			 else
			 {
			 	 int tmp_length;
 	             printf("������Ҫ��ȡ�ĳ��ȡ�\n");
 	             scanf("%d",&tmp_length);
				 my_read(tmp_fd,tmp_length);
				 tmp_fd=-1;
			 }
	 } 
   else if(!strcmp(cmd,"my_rm")) 
    {
    	   if(!param)
        	 {
			    printf("��������Ϊ��\n");
			 }
           my_rm(param);
	}  
   else if(!strcmp(cmd,"my_exitsys"))
    {
      	   my_exitsys();
	}
	else
	{
	   printf("�޷��ҵ�������\n\n");
	}

}

main()
{
   startsys();
   while(strcmp(cmd,"my_exitsys")!=0)
   {
   	 printf("=====================�����˵�=======================\n"); 
     printf("1.my_format\t\t\t��ʽ��\n");//---
     printf("2.my_mkdir    <dirname>\t\t������Ŀ¼\n");//---
     printf("3.my_rmdir    <dirname>\t\tɾ����Ŀ¼\n");//---
     
     printf("4.my_create   <filename>\t�����ļ�\n");
     printf("5.my_rm       <filename>\tɾ���ļ�\n");
     printf("6.my_ls\t\t\t\t��ʾĿ¼�е����� \n");//---
     
     printf("7.my_open     <filename>\t���ļ�\n");
     printf("8.my_close    <filename>\t�ر��ļ�\n");
     printf("9.my_cd       <dirname>\t\t���ĵ�ǰĿ¼\n");//---
  
     printf("10.my_write   <filename>\tд�ļ�\n");
     printf("11.my_read    <filename>\t���ļ�\n");
     printf("12.my_exitsys\t\t\t�˳��ļ�ϵͳ\n");//---
     printf("=================================================\n");
     printf("����������\n");
     printf("%s",cmd_head);    
     //scanf("%s",&cmd);
     gets(cmd);
     solve_cmd(cmd);
  }
  
  
  
}

