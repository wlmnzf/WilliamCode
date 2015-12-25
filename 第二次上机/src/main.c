#include "fs.h"

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



//========================ljf===================================

int my_open(char *filename)
{   //打开当前目录下名为filename的文件; 
	fcb *fcbptr;
	char *fname, exname[3], *str, text[MAXTEXT];
	int rbn, fd, i;
	fname = strtok(filename, "."); 
	str = strtok(NULL, "."); 

	if (str)
		strcpy(exname, str);
	else
		strcpy(exname, "");
   
	for (i = 0; i < MAXOPENFILE; i++)
	{
		//if (strcmp(openfilelist[i].filename, fname) == 0 && strcmp(openfilelist[i].exname, exname) == 0 && i != ptrcurfd)
  		if (strcmp(openfilelist[i].filename, fname) == 0 && strcmp(openfilelist[i].exname, exname) == 0 && i != curfd)
			{
			printf("出错,该文件已经打开.\n");
			return -1;
		}
	}
	//openfilelist[ptrcurfd].count = 0;
	openfilelist[curfd].count = 0;
//	rbn = do_read(ptrcurfd, openfilelist[ptrcurfd].length, text);
	rbn = do_read(curfd, openfilelist[curfd].length, text);
	fcbptr = (fcb *)text;//？
	
	for (i = 0; i < rbn / sizeof(fcb); i++)
	{
		if (strcmp(fcbptr->filename, fname) == 0 && strcmp(fcbptr->exname, exname) == 0)
			break;
		fcbptr++;
	}
	if (i == rbn / sizeof(fcb))
	{
		printf("error,this file not exsists.\n");
		return -1;
	}
	fd = findopenfile();
	if (fd == -1)
		return -1;
	

	strcpy(openfilelist[fd].filename, fcbptr->filename);
	strcpy(openfilelist[fd].exname, fcbptr->exname);
	openfilelist[fd].attribute = fcbptr->attribute;
	openfilelist[fd].time = fcbptr->time;
	openfilelist[fd].date = fcbptr->date;
	openfilelist[fd].first = fcbptr->first;
	openfilelist[fd].length = fcbptr->length;
	openfilelist[fd].free = fcbptr->free;
//	openfilelist[fd].dirno = openfilelist[ptrcurfd].first;
	openfilelist[fd].dirno = openfilelist[curfd].first;
	openfilelist[fd].diroff = i;

	//strcpy(openfilelist[fd].dir, openfilelist[ptrcurfd].dir);
	strcpy(openfilelist[fd].dir[fd], openfilelist[curfd].dir[curfd]);
	//strcat(openfilelist[fd].dir, filename);
	strcat(openfilelist[fd].dir[fd], filename);
	if (fcbptr->attribute & 0)
	//strcat(openfilelist[fd].dir, "\\");
	strcat(openfilelist[fd].dir[fd], "\\");
	//openfilelist[fd].father = ptrcurfd;
	openfilelist[fd].father = curfd;
	openfilelist[fd].count = 0;
	openfilelist[fd].fcbstate = 0; 
	openfilelist[fd].topenfile = 1; 
	printf("(directory/file is opened successfully)\n");
	return fd; 
}
/*
int findopenfile()
{ // 寻找空的用户打开表项 ; 
	int i;
	for (i = 0; i < MAXOPENFILE; i++)
	{
		if (openfilelist[i].topenfile == 0)
			return i;
	}
	printf("Error,open too many files!\n");

	return -1;
}*/

int my_close(int fd)
{  
	fcb *fcbptr;
	int father;
	if (fd < 0 || fd >= MAXOPENFILE)
	{
		printf("Error,the file is not exist.\n");
		return -1;
	}
	if (openfilelist[fd].fcbstate)
	{ 
		fcbptr = (fcb *)malloc(sizeof(fcb));
		strcpy(fcbptr->filename, openfilelist[fd].filename);
		strcpy(fcbptr->exname, openfilelist[fd].exname);
		fcbptr->attribute = openfilelist[fd].attribute;
		fcbptr->time = openfilelist[fd].time;
		fcbptr->date = openfilelist[fd].date;
		fcbptr->first = openfilelist[fd].first;
		fcbptr->length = openfilelist[fd].length;
		fcbptr->free = openfilelist[fd].free;
		father = openfilelist[fd].father;

		openfilelist[father].count = openfilelist[fd].diroff * sizeof(fcb);
		do_write(father, (char *)fcbptr, sizeof(fcb), 2);
		free(fcbptr);
		openfilelist[fd].fcbstate = 0;
	}

	strcpy(openfilelist[fd].filename, "");
	strcpy(openfilelist[fd].exname, "");
	openfilelist[fd].topenfile = 0;
	printf("(directory/file has closed)\n");
	return father; 
}


void my_cd(char * dirname)
{
	char * dir;
	int fd;
	dir = strtok(dirname, "\\"); 
	if (strcmp(dir, ".") == 0)
		return;  
	else if (strcmp(dir, "..") == 0)
	{
		/*
if (ptrcurfd)
			ptrcurfd = my_close(ptrcurfd);  //关闭当前路径,返回值为父路径在打开表的位置; 
		return;*/
		if (curfd)
			curfd = my_close(curfd); 
		return;
	}

	else if (strcmp(dir, "root") == 0)
	{
	/*
	while (ptrcurfd)
			ptrcurfd = my_close(ptrcurfd);  //循环关闭root下的打开文件； 
		dir = strtok(NULL, "\\");  //剩下的路径；*/
		while (curfd)
			curfd = my_close(curfd);  
		dir = strtok(NULL, "\\");  
	}
	while (dir)
	{
		fd = my_open(dir); 
		if (fd != -1) 
			//ptrcurfd = fd;   //打开表项序号； 
			curfd = fd;  
		else
			return;
		dir = strtok(NULL, "\\");
	}
//	printf("目录跳转完成\n");
   printf("enter this contents now\n");
}
//==========================ljf===============================



//========================jwt===================================
int my_create(char * filename)
{

  char *fname,*exname,text[MAX_TXT_SIZE];
  char father[100];
  int fd,rbn,i,j;
  fcb *filefcb,*fcbtmp;
  time_t *now;
  struct tm *nowtime;
  unsigned short bknum;
  fat *fat1,*fatptr;
//  if((fd=findfree0())==-1)
//    return -1;
//  else openfilelist[fd].free=1;
//  for (int i = 0; i < sizeof(filename); ++i) {
//    if(filename[i]=='\\') break;
//  }
//  if(i<sizeof(filename)) {
//    for(i= sizeof(filename);filename[i]!='\\';i--);
//    for (int j = 0; j < i; ++j) {
//      father[j]=filename[j];
//    }
//    father[i]='\0';
//    my_open(father);
//  }
  bknum=findFree();
  if(bknum==END)
  {
    printf("Sorry,there's no place for you to create a file now,but you are a good guy.");
    return -1;
  }
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
void my_rm(char *filename)
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
        exit(-1);
    }
    if(!exname)
    {
        printf("Error,removing file must have a extern name.\n");
         exit(-1);
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
        exit(-1);
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

//==============================jwt===========================


//==============================wlm==========================
void startsys()
 {
   myvhard=(char*)malloc(SIZE);
   FILE *fp;
   fp=fopen(myfsys,"r");
   int res=0;
   if(fp!=NULL)
   {
     unsigned char *buff=(char*)malloc(SIZE);
     fread (buff, SIZE,1, fp);
     //res=strncmp(magicnum,buff,8);
     if(strcmp(((block0 *)buff)->magic,magicnum)==0)
     {
       res=1;
       //strcpy(myvhard,buff);
       int i=0;
       for(i=0;i<SIZE;i++)
            myvhard[i]=buff[i];
       free(buff);
     }
   }

   if(res==0)
     {
       // printf("myfsys文件系统不存在，现在开始创建文件系统\n");
        printf("myfsys not exists，creating filesystrm....\n");
        my_format();
        fp=fopen(myfsys,"w+");
        fwrite(myvhard,SIZE,1,fp);
     }

   fclose(fp);
   fp=NULL;


 //初始化各种的东西

   strcpy(openfilelist[0].filename,"root");
   strcpy(openfilelist[0].exname,default_exname);
   openfilelist[0].attribute=0;
   openfilelist[0].time=((fcb *)(myvhard+5*BLOCKSIZE))->time;
   openfilelist[0].date=((fcb *)(myvhard+5*BLOCKSIZE))->date;
   openfilelist[0].first=((fcb *)(myvhard+5*BLOCKSIZE))->first;
   openfilelist[0].length=((fcb *)(myvhard+5*BLOCKSIZE))->length;
   openfilelist[0].free=1;
   openfilelist[0].dirno=5;
   openfilelist[0].diroff=0;
   strcpy(openfilelist[0].dir[0], "\\root\\");
   openfilelist[0].count=0;
   openfilelist[0].fcbstate=0;
   openfilelist[0].topenfile=1;

   memset(currentdir,0,sizeof(currentdir));
   strcpy(currentdir,"\\root\\");
   strcpy(openfilelist->dir[0],currentdir);
   startp=((block0 *)myvhard)->startblock;
   ptrcurdir=&openfilelist[0];

 }

unsigned short findFree()
{
   fat *fat1=(fat *)(myvhard+BLOCKSIZE);
   fat *fat2=(fat *)(myvhard+3*BLOCKSIZE);
   int len=SIZE/BLOCKSIZE;
   int i=0;
   for(i=6;i<len;i++)
   {
      if((fat1+i)->id==FREE)
      {
        return i;
      }
   }
   printf("Can't find free FAT!\n ");
   return END;
}


int my_write(int fd)
{
  if(fd<0||fd>MAXOPENFILE)//||openfilelist[fd].topenfile==0
  {
    printf("file are not opened!");
    return -1;
  }

  int wstyle=0;
  while(wstyle<1||wstyle>3)
  {
    printf("请选择写入方式：\n");
    printf("1：截断写；\n2：覆盖写；\n3：追加写\n");
  }
  fat* fat1=(fat *)(myvhard+BLOCKSIZE);
  fat* fat2=(fat *)(myvhard+3*BLOCKSIZE);
  unsigned short first=openfilelist[fd].first;
  unsigned long length=openfilelist[fd].length;
  unsigned char *start=(unsigned char *)(myvhard+first*BLOCKSIZE);
  unsigned char *del_addr=start;
  int del_block=first;
  fat *del_fat=(fat *)(fat2+del_block);
  int i=0;

  switch(wstyle)
  {
    case 1:
      while (length>0)
      {
        if(length>BLOCKSIZE)
        {
          del_block=del_fat->id;
          del_fat->id=END;
          del_fat=(fat *)(fat2+del_block);
          for(i=0;i<BLOCKSIZE;i++,del_addr++)
          {
              length--;
              del_addr=0;
          }
        }
        else
        {
          del_fat->id=END;
          for(i=0;i<length;i++,del_addr++)
          {
             del_addr=0;
             length--;
          }
        }

      }

      openfilelist[fd].count=0;
      // openfilelist[fd].length=0;
      break;
    case 2:openfilelist[fd].count=0;
    case 3:openfilelist[fd].count=openfilelist[fd].length;
    default:wstyle=0;break;
  }

    char *text=(char*)malloc(BLOCKSIZE);
    char *tmp_text=(char * )malloc(BLOCKSIZE);
    memset(text,0,BLOCKSIZE);

    printf("please input write data:\n(end with string 'exit')");
    while(1)
    {
        scanf("%s",tmp_text);
        if(strcmp(tmp_text,"exit"))
          {
              printf("write end!\n");
          }
        strcat(text,tmp_text);
        strcat(text,"\n");
    }

    int len;
    if(len=do_write(fd,text,strlen(text),wstyle)<0)
    {
      printf("write error,please input write data again!\n");
    }

    if(openfilelist[fd].count>openfilelist[fd].length)
    {
      openfilelist[fd].length=openfilelist[fd].count;
    }
    openfilelist[fd].fcbstate=1;
    return len;

}

int do_write(int fd,char *text,int len,char wstyle)
{
    unsigned char *buf=(unsigned char *)malloc(BLOCKSIZE);
    if(buf==NULL)
    {
      printf("initial buf error!\n");
      return -1;
    }

    if(fd<0||fd>MAXOPENFILE)
    {
      printf("file is not opened!\n");
      return -1;
    }

    unsigned short first_block=openfilelist[fd].first;
    unsigned short first_write_block=first_block;
    fat *fat1 =(fat *)(myvhard+BLOCKSIZE);
    fat *fat2 =(fat *)(myvhard+3*BLOCKSIZE);
    fat *first_fat=fat2+first_block;
    fat *first_write_fat=first_fat;

    int offset=openfilelist[fd].count;

    while(offset>BLOCKSIZE)
    {
      unsigned short id;
       if(first_write_fat->id==END)
       {
         id=findFree();
         if(id==END)
           return -1;

         first_write_fat->id=id;
         first_write_block=id;
         first_write_fat=(fat2+id);
         first_write_fat->id=END;
       }
       else
       {
         id=first_write_fat->id;
         first_write_block=id;
         first_write_fat=(fat2+id);
       }

       offset-=BLOCKSIZE;
    }

    unsigned char *first_write_addr=(unsigned char *)(myvhard+first_write_block*BLOCKSIZE);
    int real_len=0;
    while(len>0)
    {
      if(wstyle==2||offset>0)
      {
        strcpy(buf,first_write_addr);
      }
      else
      {
        memset(buf,0,BLOCKSIZE);
      }

      unsigned char *write_addr=first_write_addr;
      int i=0;
      if(len>BLOCKSIZE-offset)
      {
           for(write_addr=buf+offset,i=0;offset<BLOCKSIZE;i++,write_addr++,offset++,first_write_addr++,len--)
           {
             *first_write_addr=*write_addr;
             if(len>0) 
                real_len++;
           }
           //strcpy(first_write_addr,buf);
           offset=0;
           if(len>0)
            {
              first_write_block=first_write_fat->id;
              if(first_write_block==END)
              {
                first_write_block=findFree();
                if(first_write_block==END)
                  return -1;
                first_write_fat=fat2+first_write_block;
                first_write_fat->id=END;
                first_write_addr=(unsigned char *)(myvhard+first_write_block*BLOCKSIZE);

              }
              else
              {
                first_write_fat=fat2+first_write_block;
                first_write_addr=(unsigned char *)(myvhard+first_write_block*BLOCKSIZE);
              }
            }
      }
      else
      {
        for(write_addr=buf+offset,i=0;offset<len;i++,write_addr++,offset++,first_write_addr++,len--)
        {
          *first_write_addr=*write_addr;
          if(len>0)
             real_len++;
        }
        offset=0;
      }



    }

    free(buf);
  	if(openfilelist[fd].count>openfilelist[fd].length)
  	{
  		openfilelist[fd].length=openfilelist[fd].count;
  	}
  	openfilelist[fd].fcbstate=1;
  	return real_len;
}

int my_read (int fd, int len)
{
     int i=0;
     unsigned char *text=(unsigned char *)malloc(len);
     if(fd<0||fd>MAXOPENFILE)//||openfilelist[fd].topenfile==0
     {
       printf("file are not opened!");
       return -1;
     }

     openfilelist[fd].count=0;
     int res=do_read(fd,openfilelist[fd].length,text);
     if(res>=0)
     {
       printf("read content：\n %s\n",text);
     }
     else
     {
       printf("read error!\n");
     }
     free(text);
     return res;
}

int do_read (int fd, int len,char *text)//我的block都是0开始算的
{

  if(fd<0||fd>MAXOPENFILE)//||openfilelist[fd].topenfile==0
  {
    printf("file are not opened!");
    return -1;
  }
  unsigned char *buf=(unsigned char*)malloc(BLOCKSIZE);
  //strcpy(buf,"");
  int i=0;
  for(i=0;i<BLOCKSIZE;i++)
      *(buf+i)=0;
  
  unsigned short start=openfilelist[fd].first;
  unsigned long length=openfilelist[fd].length;
  unsigned short start_read_block=start;
  int offset=openfilelist[fd].count;

  fat *fat1=(fat *)(myvhard+BLOCKSIZE);
  fat *fat2=(fat *)(myvhard+3*BLOCKSIZE);
  fat *start_fat=fat2+start;
  fat *start_read_fat=start_fat;

  while(offset>BLOCKSIZE)
  {
     offset-=BLOCKSIZE;
     if(start_read_fat->id==END)
     {
       printf("file is wrong\n");
       return -1;
     }
     start_read_block=start_read_fat->id;
     start_read_fat=(fat *)(fat2+start_read_block);
  }

  unsigned char *start_addr=myvhard+start*BLOCKSIZE;
  unsigned char *start_read_addr=myvhard+start_read_block*BLOCKSIZE;

  unsigned char *buff=(unsigned char *)malloc(BLOCKSIZE);
  //strncpy(buff,start_read_addr,BLOCKSIZE);
  for(i=0;i<BLOCKSIZE;i++)
  {
     *(buff+i)=*(start_read_addr+i);
  }

  int real_len=0;
  unsigned char *read_offset;
  while(len>0)
  {
       if(BLOCKSIZE-offset > len)
    		{
    			for(read_offset=buff+offset;len>0;read_offset++,text++,len--,offset++)
    			{
    				*text=*read_offset;
    				//;
    				//offset++;
    				openfilelist[fd].count++;
    				real_len++;
    			}
    		}
    		else
    		{
    			for(read_offset=buff+offset;read_offset<buf+BLOCKSIZE;read_offset++,text++)
    			{
    				*text=*read_offset;
    				len--;
    				openfilelist[fd].count++;
    				real_len++;
    			}

    			offset=0;

    			start_read_block=start_read_fat->id;
    			start_read_fat = fat2+start_read_block;
    			start_read_addr=(unsigned char *)(myvhard+start_read_block*BLOCKSIZE);

          //strncpy(buff,start_read_addr,BLOCKSIZE);

    			 for(i=0;i<BLOCKSIZE;i++)
    			 {
    			 	*(buff+i)=*(start_read_addr+i);
    			 }
    		}
    	}
    	free(buff);
    	return real_len;
  }


// }

void my_exitsys()
{
  FILE *fp;
  fp=fopen(myfsys,"w+");
  fwrite(myvhard,SIZE,1,fp);
  close(fp);
  fp=NULL;
  int i=0;
  for(i=0;i<MAXOPENFILE;i++)
  {
     if(openfilelist[i].topenfile==0)
         my_close(i);
  }
  free(myvhard);
}



//==============================wlm======================




//==============================wgh====================
unsigned short findblock()
{
    unsigned short i;
	fat *fat1, *fatptr;
	fat1 = (fat *)(myvhard + BLOCKSIZE);
	for(i = 7; i < SIZE / BLOCKSIZE; i++)
	{
		fatptr = fat1 + i;
		if(fatptr->id == FREE)
			return i;
	}
	printf("Error,Can't find free block!\n");
//	return -1;
   return END;
}


void my_format()
{
    FILE *fp;
	fat *fat1, *fat2;
	block0 *blk0;
	time_t now;			
	struct tm *nowtime;		
	fcb *root;
	int i;
	blk0 = (block0 *)myvhard;		
	fat1 = (fat *)(myvhard + BLOCKSIZE);	
	fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);
	root = (fcb *)(myvhard + 5 * BLOCKSIZE);
	

	strcpy(blk0->magic, "10101010");		
	strcpy(blk0->information, "My FileSystem Ver 1.0 \n Blocksize=1KB Whole size=1000KB Blocknum=1000 RootBlocknum=2\n");	//写入文件系统的描述信息
	blk0->root = 5;								
	blk0->startblock = (unsigned char *)root;	
	for(i = 0; i < 5; i++)	
    {
        fat1->id = END;
        fat2->id = END;
        fat1++;
        fat2++;
    }
    fat1->id = 6;					
    fat2->id = 6;
    fat1++;
    fat2++;
    fat1->id = END;
    fat2->id = END;
    fat1++;							
    fat2++;
	
    for(i = 7; i < SIZE / BLOCKSIZE; i++)
    {
        fat1->id = FREE;
        fat2->id = FREE;
        fat1++;
        fat2++;
    }

    now = time(NULL);		
    nowtime = localtime(&now);	
	
    strcpy(root->filename, ".");   	
    strcpy(root->exname, "");
    //root->attribute = 1;	
	root->attribute = 0;		
    root->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	root->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	root->first = 5;
	root->length = 2 * sizeof(fcb);
	root->free = 1;				
	root++;
	

	now = time(NULL);		
    nowtime = localtime(&now);	
	strcpy(root->filename, "..");
    strcpy(root->exname, "");
    root->attribute = 0;
    root->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	root->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	root->first = 5;
	root->length = 2 * sizeof(fcb);
	root->free = 1;		
	
    fp = fopen(myfsys, "w");
    fwrite(myvhard, SIZE, 1, fp);
    fclose(fp);
}

void my_mkdir(char *dirname)
{
    fcb *fcbptr;
    fat *fat1, *fat2;
    time_t now;
	struct tm *nowtime;
    char text[MAXTEXT];		
    unsigned short blkno;
    int rbn, fd, i;	
	
    fat1 = (fat *)(myvhard + BLOCKSIZE);
    fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);
	
    openfilelist[curfd].count = 0;	//////???????
	
    rbn = do_read(curfd, openfilelist[curfd].length, text); 
	
    fcbptr = (fcb *)text; 
	
    for(i = 0; i < rbn / sizeof(fcb); i++)		
    {
        if(strcmp(fcbptr->filename, dirname) == 0 && strcmp(fcbptr->exname, "") == 0)
        {
            printf("Error,the dirname is already exist!\n");
            return;
        }
        fcbptr++;
    }
	
    fcbptr = (fcb *)text;   
	
    for(i = 0; i < rbn / sizeof(fcb); i++)	
    {
        if(fcbptr->free == 0)
            break;			
        fcbptr++;
    }
    blkno=findblock();		
    //if(blkno == -1)
     if(blkno == END)
        return;
    (fat1 + blkno)->id = END;	
    (fat2 + blkno)->id = END;


    now = time(NULL);
    nowtime = localtime(&now);
    strcpy(fcbptr->filename, dirname);
    strcpy(fcbptr->exname, "");
    fcbptr->attribute = 0;		
    fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	fcbptr->first = blkno;					
	fcbptr->length = 2 * sizeof(fcb);//
	//fcbptr->length = 2;
	fcbptr->free = 1;							
	openfilelist[curfd].count = i * sizeof(fcb);	
	do_write(curfd, (char *)fcbptr, sizeof(fcb), 2);		


  //=====new_added============
 /*
 int findFreeO()
{
	int i;
	for(i=0;i<MAX_OPEN_FILE;i++)
	{
		if(openfilelist[i].free==0)
		{
			return i;
		}
	}
	printf("Error,open too many files!\n");
	return -1;
}


    int free_fd=findFreeO(); 
    if(free_fd==-1)
    {
    	printf("打开的文件数超出限制\n"); 
        return;
    }
  
  
    strcpy(openfilelist[free_fd].filename,dirname);
    strcpy(openfilelist[free_fd].exname,default_exname);
    openfilelist[free_fd].attribute=0;
    openfilelist[free_fd].time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
    openfilelist[free_fd].date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
    openfilelist[free_fd].first= blkno;	
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
    openfilelist[curfd].count=(openfilelist[curfd].length-1)*sizeof(fcb);*/
    
    //=====new_added===========

	fd = my_open(dirname);	
	if(fd == -1)
        return;


	fcbptr = (fcb *)malloc(sizeof(fcb));
    now = time(NULL);
    nowtime = localtime(&now);
	strcpy(fcbptr->filename, ".");
	strcpy(fcbptr->exname, "");
	fcbptr->attribute = 0;
	fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	fcbptr->first = blkno;
	fcbptr->length = 2 * sizeof(fcb);
	fcbptr->free = 1;
	do_write(fd, (char *)fcbptr, sizeof(fcb), 2);
	
	now = time(NULL);
    nowtime = localtime(&now);
	strcpy(fcbptr->filename, "..");
	strcpy(fcbptr->exname, "");
	fcbptr->attribute = 0;
	fcbptr->time = nowtime->tm_hour * 2048 + nowtime->tm_min * 32 + nowtime->tm_sec / 2;
	fcbptr->date = (nowtime->tm_year - 80) * 512 + (nowtime->tm_mon + 1) * 32 + nowtime->tm_mday;
	fcbptr->first = blkno;
	fcbptr->length = 2 * sizeof(fcb);
	fcbptr->free = 1;
	do_write(fd, (char *)fcbptr, sizeof(fcb), 2);
	free(fcbptr);
	my_close(fd);

	fcbptr = (fcb *)text;   					
	fcbptr->length = openfilelist[curfd].length;
	openfilelist[curfd].count = 0;
	do_write(curfd, (char *)fcbptr, sizeof(fcb), 2);    
    openfilelist[curfd].fcbstate = 1;	
}

void my_rmdir(char *dirname)
{
    fcb *fcbptr,*fcbptr2;
    fat *fat1, *fat2, *fatptr1, *fatptr2;
	char text[MAXTEXT], text2[MAXTEXT];
	unsigned short blkno;
	int rbn, rbn2, fd, i, j;

	fat1 = (fat *)(myvhard + BLOCKSIZE);
    fat2 = (fat *)(myvhard + 3 * BLOCKSIZE);
	
	if(strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)		
	{
	    printf("Error,can't remove this directory.\n");
	    return;
	}
	openfilelist[curfd].count = 0;
	rbn = do_read(curfd, openfilelist[curfd].length, text);
	fcbptr = (fcb *)text;
	for(i = 0; i < rbn / sizeof(fcb); i++)
	{
	    if(strcmp(fcbptr->filename, dirname) == 0 && strcmp(fcbptr->exname, "") == 0)	
            break;
	    fcbptr++;
	}
	if(i == rbn / sizeof(fcb))
	{
	    printf("Error,the directory is not exist.\n");
        return;
	}
	fd = my_open(dirname);		
	rbn2 = do_read(fd, openfilelist[fd].length, text2);  
	fcbptr2 = (fcb *)text2;
	for(j = 0; j < rbn2 / sizeof(fcb); j++)
	{
	    if(strcmp(fcbptr2->filename, ".") && strcmp(fcbptr2->filename, "..") && strcmp(fcbptr2->filename, ""))
	    {
	        my_close(fd);
	        printf("Error,the directory is not empty.\n");
			return;
	    }
	    fcbptr2++;
	}
    blkno = openfilelist[fd].first;
    while(blkno != END)		
    {
        fatptr1 = fat1 + blkno;
        fatptr2 = fat2 + blkno;
        blkno = fatptr1->id;
        fatptr1->id = FREE;
        fatptr2->id = FREE;
    }
    my_close(fd);
	

    strcpy(fcbptr->filename, "");
    fcbptr->free = 0;
    openfilelist[curfd].count = i * sizeof(fcb);
    do_write(curfd, (char *)fcbptr, sizeof(fcb), 2);
    openfilelist[curfd].fcbstate = 1;	
}


int findopenfile()
{
    int i;
    for(i = 0; i < MAXTEXT; i++)
    {
        if(openfilelist[i].topenfile == 0)
            return i;
    }
    printf("Error,open too many files!\n");
    return -1;
}

//=============================wgh======================