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
     gets(cmd);
     solve_cmd(cmd);
  }

}

