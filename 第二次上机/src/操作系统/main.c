#include "fs.h"
#include "wlm.h"
#include "jwt.h"
#include "wgh.h"
#include "ljf.h"

int my_create(char * filename)
{
  char *fname,*exname,text[MAX_TXT_SIZE];
  char father[100];
  char fathername[100];
  char file[100];
  int fd,rbn,i,j,father_fd;
  fcb *filefcb,*fcbtmp;
  time_t *now;
  struct tm *nowtime;
  unsigned short bknum;
  fat *fat1,*fatptr;
  if((fd=findopenfile())==-1)
    return -1;
  else 
    openfilelist[fd].free=1;

  for (i = 0; i < sizeof(filename); ++i) 
  	{
        if(filename[i]=='\\') break;
    }
    
  if(i<sizeof(filename)) 
  	{
	    for(i= sizeof(filename);filename[i]!='\\';i--);
	    for (j = 0; j < i; ++j) {
	      father[j]=filename[j];
	    }
	    father[i]='\0';
	    for(i= sizeof(father);father[i]!='\\';i--);
	    i++;
	    for (j = 0; father[i]!='\0'; ++j,++i) {
	      fathername[j]=father[i];
	    }
	    fathername[++j]='\0';
	    for(i=0;i<MAXOPENFILE;i++)
	    {
	      if(strcmp(fathername,openfilelist[i].filename)==0) {father_fd=i;break;}
	    }
	    if(i==MAXOPENFILE)
	      father_fd= my_open(father);
	    if(father_fd==-1) {printf("can't open father dir!\n");return -1;}
	    for(i= sizeof(filename);filename[i]!='\\';i--);
	    i++;
	    for (j = 0; filename[i]!='\0'; ++j,++i) {
	      file[j]=filename[i];
	    }
	    file[++j]='\0';
   }
  else
   {
	    strcpy(file,filename);
	    father_fd=curfd;
   }
   
  bknum=findblock();
  if(bknum==END)
  {
	    printf("Sorry,there's no place for you to create a file now,but you are a good guy.");
	    openfilelist[fd].free=0;
	    return -1;
  }
  
  fat1=(fat *)(myvhard+BLOCKSIZE);
  fname=strtok(file,".");
  exname=strtok(NULL,".");
  
  if(strcmp(fname,"")==0)
  {
	    printf("Error,creating file must have a right name.\n");
	      openfilelist[fd].free=0;
	    return -1;
  }
  
  if(!exname)
  {
	    printf("Error,creating file must have a extern name.\n");
        openfilelist[fd].free=0;
	    return -1;
  }

  openfilelist[curfd].count=0;
  rbn=do_read(father_fd,openfilelist[father_fd].length,text);
  filefcb=(fcb *)text;
  
  for(i=0;i<rbn/sizeof(fcb);i++)
  {
    if(strcmp(fname,filefcb->filename)==0 && strcmp(exname,filefcb->exname)==0)
    {
      printf("Error,the filename is already exist!\n");
        openfilelist[fd].free=0;
      return -1;
    }
    filefcb++;
  }

  filefcb=(fcb *)text;
  for(i=0;i<rbn/sizeof(fcb);i++)
  {
    if(strcmp(filefcb->filename,"")==0)
       break;
    filefcb++;
  }
  openfilelist[father_fd].count=i*sizeof(fcb);


  fcbtmp=(fcb *)malloc(sizeof(fcb));
  now=(time_t *)malloc(sizeof(time_t));

  fcbtmp->attribute=1;
  time(now);
  nowtime=localtime(now);
  fcbtmp->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
  fcbtmp->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
  strcpy(fcbtmp->filename,fname);
  strcpy(fcbtmp->exname,exname);
  fcbtmp->first=bknum;
  fcbtmp->length=0;
  openfilelist[father_fd].fcbstate=1;
  do_write(father_fd,(char *)fcbtmp,sizeof(fcb),2);
  
  
  strcpy(openfilelist[fd].filename,fname);
  strcpy(openfilelist[fd].exname,exname);
  openfilelist[fd].attribute=1;
  openfilelist[fd].time=fcbtmp->time;
  openfilelist[fd].date=fcbtmp->date;
  openfilelist[fd].length=0;
  openfilelist[fd].dirno=openfilelist[father_fd].first;
  openfilelist[fd].diroff=i;
  openfilelist[fd].count=0;
  openfilelist[fd].fcbstate=0;
  openfilelist[fd].topenfile=1;
  free(fcbtmp);
  free(now);
  
  
  openfilelist[father_fd].count=0;
  do_read(father_fd,openfilelist[father_fd].length,text);
  
  fcbtmp=(fcb *)text;
  fcbtmp->length=openfilelist[father_fd].length;
  openfilelist[father_fd].count=0;
  do_write(father_fd,text,openfilelist[father_fd].length,2);
  openfilelist[father_fd].fcbstate=1;
  
    return fd;
}

void my_ls(){
    fcb *fcbptr;
    int i;
    char text[MAX_TXT_SIZE];
    unsigned short bknum;
    openfilelist[curfd].count=0;
    do_read(curfd,openfilelist[curfd].length,text);
    fcbptr=(fcb *)text;
    for(i=0;i<(int)(openfilelist[curfd].length/sizeof(fcb));i++)
    {
        if(fcbptr->filename[0]!='\0')
        {
            if(fcbptr->attribute==0)
            {
                printf("%s\\\t\t<DIR>\t\t%d/%d/%d\t%02d:%02d:%02d\n",fcbptr->filename,((fcbptr->date)>>9)+1980,((fcbptr->date)>>5)&0x000f,(fcbptr->date)&0x001f,fcbptr->time>>11,(fcbptr->time>>5)&0x003f,fcbptr->time&0x001f*2);
            }
            else
            {
                printf("%s.%s\t\t%dB\t\t%d/%d/%d\t%02d:%02d:%02d\t\n",fcbptr->filename,fcbptr->exname,fcbptr->length,((fcbptr->date)>>9)+1980,(fcbptr->date>>5)&0x000f,fcbptr->date&0x1f,fcbptr->time>>11,(fcbptr->time>>5)&0x3f,fcbptr->time&0x1f*2);
            }
        }
        fcbptr++;
    }
    openfilelist[curfd].count=0;


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
     gets(cmd);
     solve_cmd(cmd);
  }

}

