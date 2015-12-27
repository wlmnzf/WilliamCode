#include "fs.h"
#include "wlm.h"
#include "jwt.h"
#include "wgh.h"
#include "ljf.h"

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
     gets(cmd);
     solve_cmd(cmd);
  }

}

