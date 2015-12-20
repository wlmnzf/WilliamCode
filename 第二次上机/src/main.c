#include "fs.h"
#include "op.h"
#include "wlm.h"
main()
{
   //初始化变量
   startsys();
   int buff=0;
   while(buff!=13)
   {
     printf("1.格式化 my_format\n");
     printf("2.创建子目录 my_mkdir\n");
     printf("3.删除子目录 my_rmdir\n");
     printf("4.显示目录中的内容 my_ls\n");
     printf("5.更改当前目录 my_cd\n");
     printf("6.创建文件 my_create\n");
     printf("7.打开文件 my_open\n");
     printf("8.关闭文件 my_close\n");
     printf("9.写文件 my_write\n");
     printf("10.读文件 my_read\n");
     printf("11.删除文件 my_rm\n");
     printf("12.退出文件系统 my_exitsys\n");

     printf("请输入操作的数字\n");
     scanf("%d",&buff);
     if(buff<1&&buff>12)
        continue;

     char text[100];
     switch(buff)
      {
          case 1:my_format();break;
          case 2:
              printf("请输入文件夹名称");
              scanf("%s",text);
              my_mkdir(text);
          break;
          case 3:
              printf("请输入文件夹名称");
              scanf("%s",text);
              my_rmdir(text);
              break;
          case 4:my_ls();break;
          case 5:
              printf("请输入文件夹名称");
              scanf("%s",text);
              my_cd(text);
          break;
          case 6:
            printf("请输入文件名");
            scanf("%s",text);
            my_create(text);
            break;
          case 7:
              printf("请输入文件名");
              scanf("%s",text);
              my_open(text);
              break;
          // case 8:my_close();break;
          // case 9:my_write();break;
          // case 10:my_read();break;
          case 11:
              printf("请输入文件名");
              scanf("%s",text);
              my_rm(text);
              break;
          default:buff=12;my_exitsys();break;
      }
  }


}



void my_mkdir(char *dirname)
{
	char *text=(char *)malloc(BLOCKSIZE);
	int curfd=find_fd_by_openfile(ptrcurdir->filename,ptrcurdir->exname);
	if(curfd==-1)  
	{
		printf("找不到当前目录\n"); 
		return;
	} 
	openfilelist[curfd].count=0;
    int res= do_read(curfd,ptrcurdir->length,text);
    if(res==-1)  
	{
		printf("读取内容失败，文件已损坏\n"); 
	    return;
    } 
   
   
//    int i=0;
//    for(i=0;i<MAXOPENFILE;i++)
//    {
//		if()
//	}
 
    
    fcb *first_fcb=(fcb *)text; 
    fcb *tmp_fcb=first_fcb;
    
    
    int fcbNum=res/sizeof(fcb);
    for(i=0;i<fcbNum;i++)
    {
       if(strcmp(tmp_fcb->filename,dirname)==0&&strcmp(tmp_fcb->exname,exname)==0)
          {
              printf("已存在该文件夹！\n");
              return;
          }
       tmp_fcb++;
    }
    
    
    int free_fd=findFreeO(); 
    if(free_fd==-1)
    {
    //	printf("打开的文件数超出限制\n"); 
        return;
    }
    openfilelist[curfd].count=i*sizeof(fcb);
    
    int free_block=findFree();
    if(free_block==END) 
	   return;
   
   openfilelist[curfd].length+=1;
   
   
   	time(now);
	nowtime=localtime(now);
    fcb *new_fcb=(fcb *)malloc(sizeof(fcb));
    strcpy(new_fcb->filename,dirname);
    strcpy(new_fcb->exname,exname);
    new_fcb->attribute=0;
	new_fcb->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
	new_fcb->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
	new_fcb->first=free_block;
	new_fcb->length=2;
	new_fcb->free =1;
    
    strcpy(openfilelist[free_fd].filename,dirname);
    strcpy(openfilelist[free_fd].exname,exname);
    openfilelist[free_fd].attribute=0;
    openfilelist[free_fd].time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
    openfilelist[free_fd].date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
    openfilelist[free_fd].first=free_block;
    openfilelist[free_fd].length=2;
	openfilelist[free_fd].free=1;
	openfilelist[free_fd].dirno=openfilelist[curfd].first;
	strcpy(openfilelist[free_fd].dir[free_fd],openfilelist[curfd].dir[curfd]);
	strcat(openfilelist[free_fd].dir[free_fd],dirname);
	strcat(openfilelist[free_fd].dir[free_fd],"\\");
	openfilelist[free_fd].diroff=i;
	openfilelist[free_fd].count=0;
	openfilelist[free_fd].fcbstate=1;
	openfilelist[free_fd].topenfile=1;
	
    openfilelist[curfd].length+=1;
    openfilelist[curfd].count=(openfilelist[curfd].length-1)*sizeof(fcb);
    res=do_write(curfd,(char *)new_fcb,sizeof(fcb),2);
    if(res=-1)
    {
	   printf("写入文件夹失败\n");
	   return;
	}
	
	fat *fat1=(fat *)(myvhard+BLOCKSIZE);
	fat *fat2=(fat *)(myvhard+3*BLOCKSIZE);
	
	
	(fat1+free_block)->id=END;
	(fat2+free_block)->id=END;
	
	new_fcb=(fcb *)malloc(sizeof(fcb));
	time(now);
	nowtime=localtime(now);
    strcpy(new_fcb->filename,".");
    strcpy(new_fcb->exname,exname);
    new_fcb->attribute=0;
	new_fcb->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
	new_fcb->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
	new_fcb->first=free_block;
	new_fcb->length=0;
	new_fcb->free =0;
	
    res=do_write(free_fd,(char *)new_fcb,sizeof(fcb),2);
    if(res==-1)
      return;
      
    strcpy(new_fcb->filename,"..");
    res=do_write(free_fd,(char *)new_fcb,sizeof(fcb),2);
    if(res==-1)
      return;
	//openfilelist[free_fd].father=curfd;    

}







void my_ls()
{
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
			if(fcbptr->attribute&0x20)
			{
				printf("%s\\\t\t<DIR>\t\t%d/%d/%d\t%02d:%02d:%02d\n",fcbptr->filename,((fcbptr->date)>>9)+1980,((fcbptr->date)>>5)&0x000f,(fcbptr->date)&0x001f,fcbptr->time>>11,(fcbptr->time>>5)&0x003f,fcbptr->time&0x001f*2);
			}
			else
			{
			//	printf("%s.%s\t\t%dB\t\t%d/%d/%d\t%02d:%02d:%02d\t\n",fcbptr->filename,fcbptr->exname,fcbptr->length,fcbptr->date>>9+1980,fcbptr->date>>5&0x000f,fcbptr->date&0x1f,fcbptr->time>>11,fcbptr->time>>5&0x3f,fcbptr->time&0x1f*2);
			}
		}
		fcbptr++;
	}
	openfilelist[curfd].count=0;
}


void my_rmdir(char *dirname)
{
	int rbn,fd;
	char text[MAX_TXT_SIZE];
	fcb *fcbptr,*fcbtmp,*fcbtmp2;
	unsigned short bknum;
	int i,j;
	fat *fat1,*fatptr;

	if(strcmp(dirname,".")==0 || strcmp(dirname,"..")==0)
	{
		printf("Error,can't remove this directory.\n");
		return;
	}
	fat1=(fat *)(myvhard+BLOCKSIZE);
	openfilelist[curfd].count=0;
	rbn=do_read(curfd,openfilelist[curfd].length,text);
	fcbptr=(fcb *)text;
	for(i=0;i<rbn/sizeof(fcb);i++)
	{
		if(strcmp(dirname,fcbptr->filename)==0)
		{
			break;
		}
		fcbptr++;
	}
	if(i >= rbn/sizeof(fcb))
	{
		printf("Error,the directory is not exist.\n");
		return;
	}

	bknum=fcbptr->first;
	fcbtmp2=fcbtmp=(fcb *)(myvhard+bknum*BLOCKSIZE);
	for(j=0;j<fcbtmp->length/sizeof(fcb);j++)
	{
		if(strcmp(fcbtmp2->filename,".") && strcmp(fcbtmp2->filename,"..") && fcbtmp2->filename[0]!='\0')
		{
			printf("Error,the directory is not empty.\n");
			return;
		}
		fcbtmp2++;
	}

	while(bknum!=END)
	{
		fatptr=fat1+bknum;
		bknum=fatptr->id;
		fatptr->id=FREE;
	}

	strcpy(fcbptr->filename,"");
	strcpy(fcbptr->exname,"");
	fcbptr->first=END;
	openfilelist[curfd].count=0;
	do_write(curfd,text,openfilelist[curfd].length,2);

}


/*
原型声明:		int my_open(char *filename)
功能描述：		打开文件函数
输入：
		filename		指向要打开的文件的名字的指针
输出：			返回打开的文件的id

函数功能实现算法描述：

*/
int my_open(char *filename)
{
	int i,fd,rbn;
	char text[MAX_TXT_SIZE],*p,*fname,*exname;
	fcb *fcbptr;
	char exnamed=0;
	fname=strtok(filename,".");
	exname=strtok(NULL,".");
	if(!exname)
	{
		exname=(char *)malloc(3);
		strcpy(exname,"di");
		exnamed=1;
	}
	for(i=0;i<MAX_OPEN_FILE;i++)
	{
		if(strcmp(openfilelist[i].filename,filename)==0 &&strcmp(openfilelist[i].exname,exname)==0&& i!=curfd)
		{
			printf("Error,the file is already open.\n");
			return -1;
		}
	}
	openfilelist[curfd].count=0;
	rbn=do_read(curfd,openfilelist[curfd].length,text);
	fcbptr=(fcb *)text;

	for(i=0;i<rbn/sizeof(fcb);i++)
	{
		if(strcmp(filename,fcbptr->filename)==0 && strcmp(fcbptr->exname,exname)==0)
		{
				break;
		}
		fcbptr++;
	}
	if(i>=rbn/sizeof(fcb))
	{
		printf("Error,the file is not exist.\n");
		return curfd;
	}

	if(exnamed)
	{
		free(exname);
	}

	fd=findFreeO();
	if(fd==-1)
	{
		return -1;
	}

	strcpy(openfilelist[fd].filename,fcbptr->filename);
	strcpy(openfilelist[fd].exname,fcbptr->exname);
	openfilelist[fd].attribute=fcbptr->attribute;
	openfilelist[fd].count=0;
	openfilelist[fd].date=fcbptr->date;
	openfilelist[fd].first=fcbptr->first;
	openfilelist[fd].length=fcbptr->length;
	openfilelist[fd].time=fcbptr->time;
	openfilelist[fd].father=curfd;
	openfilelist[fd].dirno=openfilelist[curfd].first;
	openfilelist[fd].diroff=i;
	openfilelist[fd].fcbstate=0;
	openfilelist[fd].free=1;
	openfilelist[fd].topenfile=1;
	strcpy(openfilelist[fd].dir[fd],openfilelist[curfd].dir[curfd]);
	p=openfilelist[fd].dir[fd];
	while(*p!='\0')
		p++;
	strcpy(p,filename);
	while(*p!='\0') p++;
	if(openfilelist[fd].attribute&0x20)
	{
		*p='\\';p++;	*p='\0';

	}
	else
	{
		*p='.';p++;
		strcpy(p,openfilelist[fd].exname);
	}

	return fd;
}

/**/
/*
原型声明:		int my_close(int fd)
功能描述：		关闭文件函数
输入：
		fd		打开文件的id
输出：			返回fd的father的id

函数功能实现算法描述：

*/
int my_close(int fd)
{
	fcb *fafcb;
	char text[MAX_TXT_SIZE];
	int fa;
	/*
	检查fd的有效性
	*/
	if(fd > MAX_OPEN_FILE || fd<=0)
	{
		printf("Error,the file is not exist.\n");
		return -1;
	}

	fa=openfilelist[fd].father;
	if(openfilelist[fd].fcbstate)
	{
		fa=openfilelist[fd].father;
		openfilelist[fa].count=0;
		do_read(fa,openfilelist[fa].length,text);

		fafcb=(fcb *)(text+sizeof(fcb)*openfilelist[fd].diroff);
		fafcb->attribute=openfilelist[fd].attribute;
		fafcb->date=openfilelist[fd].date;
		fafcb->first=openfilelist[fd].first;
		fafcb->length=openfilelist[fd].length;
		fafcb->time=openfilelist[fd].time;
		strcpy(fafcb->filename,openfilelist[fd].filename);
		strcpy(fafcb->exname,openfilelist[fd].exname);

		openfilelist[fa].count=0;
		do_write(fa,text,openfilelist[fa].length,2);

	}
	openfilelist[fd].attribute=0;
	openfilelist[fd].count=0;
	openfilelist[fd].date=0;
	strcpy(openfilelist[fd].dir[fd],"");
	strcpy(openfilelist[fd].filename,"");
	strcpy(openfilelist[fd].exname,"");
	openfilelist[fd].length=0;
	openfilelist[fd].time=0;
	openfilelist[fd].free=0;
	openfilelist[fd].topenfile=0;

	return fa;
}

/**/
/*
原型声明:		void my_cd(char *dirname)
功能描述：		更改当前目录函数
输入：
	dirname		指向目录名的指针
输出：			无

函数功能实现算法描述：

*/
int my_cd(char *dirname)
{
	char *p,text[MAX_TXT_SIZE];
	int fd,i;
	p=strtok(dirname,"\\");
	if(strcmp(p,".")==0)
		return -1;
	if(strcmp(p,"..")==0)
	{
		fd=openfilelist[curfd].father;
		my_close(curfd);
		curfd=fd;
		ptrcurdir=&openfilelist[curfd];
		return -1;
	}
	if(strcmp(p,"root")==0)
	{
		for(i=1;i<MAX_OPEN_FILE;i++)
		{
			if(openfilelist[i].free)
				my_close(i);
		}
		ptrcurdir=&openfilelist[0];
		curfd=0;
		p=strtok(NULL,"\\");
	}

	while(p)
	{
		fd=my_open(p);
		if(fd>0)
		{
			ptrcurdir=&openfilelist[fd];
			curfd=fd;
		}
		else
			return -1;
		p=strtok(NULL,"\\");
	}
}

/**/
/*
原型声明:		void my_create(char *filename)
功能描述：		创建文件函数
输入：
		filename	指向文件名的指针
输出：			无

函数功能实现算法描述：

*/
int my_create(char *filename)
{
	char *fname,*exname,text[MAX_TXT_SIZE];
	int fd,rbn,i;
	fcb *filefcb,*fcbtmp;
	time_t *now;
	struct tm *nowtime;
	unsigned short bknum;
	fat *fat1,*fatptr;

	fat1=(fat *)(myvhard+BLOCKSIZE);
	fname=strtok(filename,".");
	exname=strtok(NULL,".");
	if(strcmp(fname,"")==0)
	{
		printf("Error,creating file must have a right name.\n");
		return -1;
	}
	if(!exname)
	{
		printf("Error,creating file must have a extern name.\n");
		return -1;
	}

	openfilelist[curfd].count=0;
	rbn=do_read(curfd,openfilelist[curfd].length,text);
	filefcb=(fcb *)text;
	for(i=0;i<rbn/sizeof(fcb);i++)
	{
		if(strcmp(fname,filefcb->filename)==0 && strcmp(exname,filefcb->exname)==0)
		{
			printf("Error,the filename is already exist!\n");
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
	openfilelist[curfd].count=i*sizeof(fcb);

	bknum=findFree();
	if(bknum==END)
	{
		return -1;
	}
	fcbtmp=(fcb *)malloc(sizeof(fcb));
	now=(time_t *)malloc(sizeof(time_t));

	fcbtmp->attribute=0x00;
	time(now);
	nowtime=localtime(now);
	fcbtmp->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
	fcbtmp->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
	strcpy(fcbtmp->filename,fname);
	strcpy(fcbtmp->exname,exname);
	fcbtmp->first=bknum;
	fcbtmp->length=0;

	do_write(curfd,(char *)fcbtmp,sizeof(fcb),2);
	free(fcbtmp);
	free(now);
	openfilelist[curfd].count=0;
	do_read(curfd,openfilelist[curfd].length,text);
	fcbtmp=(fcb *)text;
	fcbtmp->length=openfilelist[curfd].length;
	openfilelist[curfd].count=0;
	do_write(curfd,text,openfilelist[curfd].length,2);
	openfilelist[curfd].fcbstate=1;
	fatptr=(fat *)(fat1+bknum);
	fatptr->id=END;
}

/**/
/*
原型声明:		void my_rm(char *filename)
功能描述：		删除文件函数
输入：
		filename	指向文件名的指针
输出：			无

函数功能实现算法描述：

*/
int my_rm(char *filename)
{
	char *fname,*exname;
	char text[MAX_TXT_SIZE];
	fcb *fcbptr;
	int i,rbn;
	unsigned short bknum;
	fat *fat1,*fatptr;

	fat1=(fat *)(myvhard+BLOCKSIZE);
	fname=strtok(filename,".");
	exname=strtok(NULL,".");
	if(!fname || strcmp(fname,"")==0)
	{
		printf("Error,removing file must have a right name.\n");
		return -1;
	}
	if(!exname)
	{
		printf("Error,removing file must have a extern name.\n");
		return -1;
	}
	openfilelist[curfd].count=0;
	rbn=do_read(curfd,openfilelist[curfd].length,text);
	fcbptr=(fcb *)text;
	for(i=0;i<rbn/sizeof(fcb);i++)
	{
		if(strcmp(fname,fcbptr->filename)==0 && strcmp(exname,fcbptr->exname)==0)
		{
			break;
		}
		fcbptr++;
	}
	if(i>=rbn/sizeof(fcb))
	{
		printf("Error,the file is not exist.\n");
		return -1;
	}

	bknum=fcbptr->first;
	while(bknum!=END)
	{
		fatptr=fat1+bknum;
		bknum=fatptr->id;
		fatptr->id=FREE;
	}

	strcpy(fcbptr->filename,"");
	strcpy(fcbptr->exname,"");
	fcbptr->first=END;
	openfilelist[curfd].count=0;
	do_write(curfd,text,openfilelist[curfd].length,2);
}



