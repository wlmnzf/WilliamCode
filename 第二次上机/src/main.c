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
			    printf("参数不能为空\n");
			 }
              my_mkdir(param);
        }
    else if(!strcmp(cmd,"my_rmdir"))
        {
     	     if(!param)
        	 {
			    printf("参数不能为空\n");
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
			    printf("参数不能为空\n");
			 }
              my_cd(param);
	   }
    else if(!strcmp(cmd,"my_crete"))
       {
       	    if(!param)
        	 {
			    printf("参数不能为空\n");
			 }
	          my_create(param);
       }
    else if(!strcmp(cmd,"my_open"))
      {
      	    if(!param)
        	 {
			    printf("参数不能为空\n");
			 }
	          my_open(param);
	  }
    else if(!strcmp(cmd,"my_close"))
     {
     	   if(!param)
        	 {
			    printf("参数不能为空\n");
			 }
			  int tmp_fd=find_fd_by_name(param);
		      if(tmp_fd==-1)
		         {
				     printf("该文件未被打开!\n");
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
			    printf("参数不能为空\n");
			 }
          int tmp_fd=find_fd_by_name(param);
          if(tmp_fd==-1)
             {
			    printf("该文件未被打开!\n");
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
			    printf("参数不能为空\n");
			 }
	       int tmp_fd=find_fd_by_name(param);
           if(tmp_fd==-1)
             {
			    printf("该文件未被打开!\n");
			 }
			 else
			 {
			 	 int tmp_length;
 	             printf("请输入要读取的长度。\n");
 	             scanf("%d",&tmp_length);
				 my_read(tmp_fd,tmp_length);
				 tmp_fd=-1;
			 }
	 } 
   else if(!strcmp(cmd,"my_rm")) 
    {
    	   if(!param)
        	 {
			    printf("参数不能为空\n");
			 }
           my_rm(param);
	}  
   else if(!strcmp(cmd,"my_exitsys"))
    {
      	   my_exitsys();
	}
	else
	{
	   printf("无法找到该命令\n\n");
	}

}

main()
{
   startsys();
   while(strcmp(cmd,"my_exitsys")!=0)
   {
   	 printf("=====================操作菜单=======================\n"); 
     printf("1.my_format\t\t\t格式化\n");//---
     printf("2.my_mkdir    <dirname>\t\t创建子目录\n");//---
     printf("3.my_rmdir    <dirname>\t\t删除子目录\n");//---
     
     printf("4.my_create   <filename>\t创建文件\n");
     printf("5.my_rm       <filename>\t删除文件\n");
     printf("6.my_ls\t\t\t\t显示目录中的内容 \n");//---
     
     printf("7.my_open     <filename>\t打开文件\n");
     printf("8.my_close    <filename>\t关闭文件\n");
     printf("9.my_cd       <dirname>\t\t更改当前目录\n");//---
  
     printf("10.my_write   <filename>\t写文件\n");
     printf("11.my_read    <filename>\t读文件\n");
     printf("12.my_exitsys\t\t\t退出文件系统\n");//---
     printf("=================================================\n");
     printf("请输入命令\n");
     printf("%s",cmd_head);    
     //scanf("%s",&cmd);
     gets(cmd);
     solve_cmd(cmd);
  }
  
  
  
}

