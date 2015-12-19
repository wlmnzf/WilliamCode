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


// void startsys()
// {
//   myvhard=(char*)malloc(SIZE);
//   FILE *fp；
//   fp=fopen(myfsys,"r");
//   int res=0;
//   if(fp!=NULL)
//   {
//     unsigned char *buff=(char*)malloc(SIZE);
//     fread (buff, SIZE,1, fp);
//     res=strncmp(magicnum,(block0 *)buff->magicnum,8);
//     //res=strncmp(magicnum,buff,8);
//     if(res==0)
//     {
//       res==1;
//       strcpy(myfsys,buff);
//       free(buff);
//     }
//   }
//
//   if(res==0)
//     {
//        printf("myfsys文件系统不存在，现在开始创建文件系统\n");
//        my_format();
//        fp=fopen(myfsys,"w+");
//        fwrite(myvhard,SIZE,1,fp);
//     }
//
//   fclose(fp);
//   fp=NULL;
//
//
// //初始化各种的东西
//
//   strcpy(openfilelist[0].filename,"root");
//   strcpy(openfilelist[0].exname,exname);
//   openfilelist[0].attribute=0;
//   openfilelist[0].time=((fcb *)(myvhard+5*BLOCKSIZE))->time;
//   openfilelist[0].date=((fcb *)(myvhard+5*BLOCKSIZE))->date;
//   openfilelist[0].first=((fcb *)(myvhard+5*BLOCKSIZE))->first;
//   openfilelist[0].length=((fcb *)(myvhard+5*BLOCKSIZE))->length;
//   openfilelist[0].free=1;
//   openfilelist[0].dirno=5;
//   openfilelist[0].diroff=0;
//   strcpy(openfilelist[0].dir, "/root/");
//   openfilelist[0].count=0;
//   openfilelist[0].fcbstate=0;
//   openfilelist[0].topenfile=1；
//
//   memset(currentdir,0,sizeof(currentdir));
//   strcpy(currentdir,"/root/");
//   strcpy(openfilelist->dir[0],currentdir);
//   startp=((block0 *)myvhard)->startblock;
//   ptrcurdir=&openfilelist[0];
//
// }
//
void my_format()
{
   block0* first_block=(block0*)myvhard;
   fat *fat1=(fat*)(first_block+BLOCKSIZE);
   fat *fat2=(fat*)(first_block+3*BLOCKSIZE);
   unsigned char *data=(unsigned char*)(myvhard+5*BLOCKSIZE);
   fcb *root=(fcb*)data;

   strcpy(first_block->magicnum,magicnum);
   strcpy(first_block->information,"William_FS V1.0\nBLOCKSIZE:1024\nBLOCKCOUNT:1000\nAUTHOR:William\n");
   first_block->root=0;
   first_block->startblock=data;

   int i=0;
   fat *tmp1=fat1;
   fat *tmp2=fat2;
   for(i=0;i<5;i++)
   {
     tmp1->id=END;
     tmp2->id=END;
     tmp1++;
     tmp2++;
   }
   int total=SIZE/BLOCKSIZE;
   for(i=6;i<total;i++)
   {
     tmp1->id=FREE;
     tmp2->id=FREE;
     tmp1++;
     tmp2++;
   }

    (fat1+5)->id=6;
    (fat2+5)->id=6;

    time_t *now;
    struct tm *nowtime;
   	strcpy(root->filename,".");
   	strcpy(root->exname,exname);
   	root->attribute=0;
   	now=(time_t *)malloc(sizeof(time_t));
   	time(now);
   	nowtime=localtime(now);
   	root->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
   	root->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
   	root->first=5;
   	root->length=2*sizeof(fcb);
   	root++;

   	strcpy(root->filename,"..");
   	strcpy(root->exname,exname);
   	root->attribute=0;
   	time(now);
   	nowtime=localtime(now);
   	root->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
   	root->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
   	root->first=5;
   	root->length=2*sizeof(fcb);
   	root++;

   	for(i=2;i<BLOCKSIZE*2/sizeof(fcb);i++,root++)
   	{
   		root->filename[0]='\0';
   	}
   FILE*	fp=fopen(filename,"w");

   	fwrite(myvhard,SIZE,1,fp);
   	free(now);
   	fclose(fp);

}

void startsys()
{
	FILE *fp;
	int i;
    unsigned char *buffer=(unsigned char*)malloc(SIZE);
	myvhard=(unsigned char *)malloc(SIZE);
	memset(myvhard,0,SIZE);
	fp=fopen(filename,"r");
	if(fp)
	{
		fread(buffer,SIZE,1,fp);
		fclose(fp);
		if(buffer[0]!=0xaa||buffer[1]!=0xaa)
		{
			printf("myfilesys is not exist,begin to creat the file...\n");
			my_format();
		}
		else
		{
			for(i=0;i<SIZE;i++)
					myvhard[i]=buffer[i];
		}
	}
	else
	{
		printf("myfilesys is not exist,begin to creat the file...\n");
		my_format();
	}


	strcpy(openfilelist[0].filename,"root");
	strcpy(openfilelist[0].exname,"di");
	openfilelist[0].attribute=0x2d;
	openfilelist[0].time=((fcb *)(myvhard+5*BLOCKSIZE))->time;
	openfilelist[0].date=((fcb *)(myvhard+5*BLOCKSIZE))->date;
	openfilelist[0].first=((fcb *)(myvhard+5*BLOCKSIZE))->first;
	openfilelist[0].length=((fcb *)(myvhard+5*BLOCKSIZE))->length;
	openfilelist[0].free=1;
	openfilelist[0].dirno=5;
	openfilelist[0].diroff=0;
	openfilelist[0].count=0;
	openfilelist[0].fcbstate=0;
	openfilelist[0].topenfile=0;
	openfilelist[0].father=0;

	memset(currentdir,0,sizeof(currentdir));
	strcpy(currentdir,"\\root\\");
	strcpy(openfilelist->dir[0],currentdir);
	startp=((block0 *)myvhard)->startblock;
	ptrcurdir=&openfilelist[0];
	curfd=0;
}

//
// void my_format()
// {
// 	FILE *fp;
// 	fat *fat1,*fat2;
// 	block0 *b0;
// 	time_t *now;
// 	struct tm *nowtime;
// 	unsigned char *p;
// 	fcb *root;
// 	int i;
//
// 	p=myvhard;
// 	b0=(block0 *)p;
// 	fat1=(fat *)(p+BLOCKSIZE);
// 	fat2=(fat*)(p+3*BLOCKSIZE);
// 	/*
// 	引导块
// 	*/
// 	b0->fbnum=0xaaaa;	/*文件系统魔数 10101010*/
// 	b0->root = 5;
// 	strcpy(b0->information,"My FileSystem Ver 1.0 \n Blocksize=1KB Whole size=1000KB Blocknum=1000 RootBlocknum=2\n");
// 	/*
// 	FAT1,FAT2
// 		前面五个磁盘块已分配，标记为END
// 	*/
// 	fat1->id=END;
// 	fat2->id=END;
// 	fat1++;fat2++;
// 	fat1->id=END;
// 	fat2->id=END;
// 	fat1++;fat2++;
// 	fat1->id=END;
// 	fat2->id=END;
// 	fat1++;fat2++;
// 	fat1->id=END;
// 	fat2->id=END;
// 	fat1++;fat2++;
// 	fat1->id=END;
// 	fat2->id=END;
// 	fat1++;fat2++;
// 	fat1->id=6;
// 	fat2->id=6;
// 	fat1++;fat2++;
// 	fat1->id=END;
// 	fat2->id=END;
// 	fat1++;fat2++;
// 	/*
// 	将数据区的标记为空闲状态
// 	*/
// 	for(i=7;i<SIZE/BLOCKSIZE;i++)
// 	{
// 		(*fat1).id=FREE;
// 		(*fat2).id=FREE;
// 		fat1++;
// 		fat2++;
// 	}
//   /*
// 创建根目录文件root，将数据区的第一块分配给根目录区
// 在给磁盘上创建两个特殊的目录项：".",".."，
// 除了文件名之外，其它都相同
// */
// p+=BLOCKSIZE*5;
// root=(fcb *)p;
// strcpy(root->filename,".");
// strcpy(root->exname,"di");
// root->attribute=40;
// now=(time_t *)malloc(sizeof(time_t));
// time(now);
// nowtime=localtime(now);
// root->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
// root->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
// root->first=5;
// root->length=2*sizeof(fcb);
// root++;
//
// strcpy(root->filename,"..");
// strcpy(root->exname,"di");
// root->attribute=40;
// time(now);
// nowtime=localtime(now);
// root->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
// root->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
// root->first=5;
// root->length=2*sizeof(fcb);
// root++;
//
// for(i=2;i<BLOCKSIZE*2/sizeof(fcb);i++,root++)
// {
//   root->filename[0]='\0';
// }
// fp=fopen(filename,"w");
// b0->startblock=p+BLOCKSIZE*4;
// fwrite(myvhard,SIZE,1,fp);
// free(now);
// fclose(fp);
//
// }


void my_mkdir(char *dirname)
{
	fcb *dirfcb,*pcbtmp;
	int rbn,i,fd;
	unsigned short bknum;
	char text[MAX_TXT_SIZE],*p;
	time_t *now;
	struct tm *nowtime;
	/*
	将当前的文件信息读到text中
	rbn 是实际读取的字节数
	*/
	openfilelist[curfd].count=0;
	rbn = do_read(curfd,openfilelist[curfd].length,text);
	dirfcb=(fcb *)text;
	/*
	检测是否有相同的目录名
	*/
	for(i=0;i<rbn/sizeof(fcb);i++)
	{
		if(strcmp(dirname,dirfcb->filename)==0)
		{
			printf("Error,the dirname is already exist!\n");
			return;
		}
		dirfcb++;
	}

	/*
	分配一个空闲的打开文件表项
	*/
	dirfcb=(fcb *)text;
	for(i=0;i<rbn/sizeof(fcb);i++)
	{
		if(strcmp(dirfcb->filename,"")==0)
			break;
		dirfcb++;
	}
	openfilelist[curfd].count=i*sizeof(fcb);
	fd=findFree();
	if(fd<0)
	{
		return;
	}

	/*
	寻找空闲盘块
	*/
	bknum = findFree();
	if(bknum == END )
	{
		return;
	}

	pcbtmp=(fcb *)malloc(sizeof(fcb));
	now=(time_t *)malloc(sizeof(time_t));

	//在当前目录下新建目录项
	pcbtmp->attribute=0x30;
	time(now);
	nowtime=localtime(now);
	pcbtmp->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
	pcbtmp->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
	strcpy(pcbtmp->filename,dirname);
	strcpy(pcbtmp->exname,"di");
	pcbtmp->first=bknum;
	pcbtmp->length=2*sizeof(fcb);

	openfilelist[fd].attribute=pcbtmp->attribute;
	openfilelist[fd].count=0;
	openfilelist[fd].date=pcbtmp->date;
	strcpy(openfilelist[fd].dir[fd],openfilelist[curfd].dir[curfd]);

	p=openfilelist[fd].dir[fd];
	while(*p!='\0')
		p++;
	strcpy(p,dirname);
	while(*p!='\0') p++;
	*p='\\';p++;
	*p='\0';

	openfilelist[fd].dirno=openfilelist[curfd].first;
	openfilelist[fd].diroff=i;
	strcpy(openfilelist[fd].exname,pcbtmp->exname);
	strcpy(openfilelist[fd].filename,pcbtmp->filename);
	openfilelist[fd].fcbstate=1;
	openfilelist[fd].first=pcbtmp->first;
	openfilelist[fd].length=pcbtmp->length;
	openfilelist[fd].free=1;
	openfilelist[fd].time=pcbtmp->time;
	openfilelist[fd].topenfile=1;

	do_write(curfd,(char *)pcbtmp,sizeof(fcb),2);

	pcbtmp->attribute=0x28;
	time(now);
	nowtime=localtime(now);
	pcbtmp->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
	pcbtmp->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
	strcpy(pcbtmp->filename,".");
	strcpy(pcbtmp->exname,"di");
	pcbtmp->first=bknum;
	pcbtmp->length=2*sizeof(fcb);

	do_write(fd,(char *)pcbtmp,sizeof(fcb),2);

	pcbtmp->attribute=0x28;
	time(now);
	nowtime=localtime(now);
	pcbtmp->time=nowtime->tm_hour*2048+nowtime->tm_min*32+nowtime->tm_sec/2;
	pcbtmp->date=(nowtime->tm_year-80)*512+(nowtime->tm_mon+1)*32+nowtime->tm_mday;
	strcpy(pcbtmp->filename,"..");
	strcpy(pcbtmp->exname,"di");
	pcbtmp->first=openfilelist[curfd].first;
	pcbtmp->length=openfilelist[curfd].length;

	do_write(fd,(char *)pcbtmp,sizeof(fcb),2);

	openfilelist[curfd].count=0;
	do_read(curfd,openfilelist[curfd].length,text);

	pcbtmp=(fcb *)text;
	pcbtmp->length=openfilelist[curfd].length;
	my_close(fd);

	openfilelist[curfd].count=0;
	do_write(curfd,text,pcbtmp->length,2);
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

	fd=findFree();
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
		return;
	if(strcmp(p,"..")==0)
	{
		fd=openfilelist[curfd].father;
		my_close(curfd);
		curfd=fd;
		ptrcurdir=&openfilelist[curfd];
		return;
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
       int id;
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

    unsigned char * first_write_addr=(unsigned char *)(myvhard+first_write_block*BLOCKSIZE);
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
             real_len++;
           }
           strcpy(first_write_addr,buf);
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
  strncpy(buff,start_read_addr,BLOCKSIZE);

  int real_len=0;
  unsigned char *read_offset;
  while(len>0)
  {
       if(BLOCKSIZE-offset > len)
    		{
    			for(read_offset=buff+offset;len>0;read_offset++,text++)
    			{
    				*text=*read_offset;
    				len--;
    				offset++;
    				openfilelist[fd].count++;
    			}
    		}
    		else
    		{
    			for(read_offset=buff+offset;read_offset<buf+BLOCKSIZE;read_offset++,text++)
    			{
    				*text=*read_offset;
    				len--;
    				openfilelist[fd].count++;
    			}

    			offset=0;

    			start_read_block=start_read_fat->id;
    			start_read_fat = fat2+start_read_block;
    			start_read_addr=(unsigned char *)(myvhard+start_read_block*BLOCKSIZE);

          strncpy(buff,start_read_addr,BLOCKSIZE);

    			// for(i=0;i<BLOCKSIZE;i++)
    			// {
    			// 	buff[i]=bkptr[i];
    			// }
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
  //还得撤消用户打开文件列表
  free(myvhard);
}
