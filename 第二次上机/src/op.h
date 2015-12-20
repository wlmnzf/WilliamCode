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
     if(strcmp(((block0 *)buff)->magicnum,magicnum)==0)
     {
       res=1;
       strcpy(myfsys,buff);
       free(buff);
     }
   }

   if(res==0)
     {
        printf("myfsys文件系统不存在，现在开始创建文件系统\n");
        my_format();
        fp=fopen(myfsys,"w+");
        fwrite(myvhard,SIZE,1,fp);
     }

   fclose(fp);
   fp=NULL;


 //初始化各种的东西

   strcpy(openfilelist[0].filename,"root");
   strcpy(openfilelist[0].exname,exname);
   openfilelist[0].attribute=0;
   openfilelist[0].time=((fcb *)(myvhard+5*BLOCKSIZE))->time;
   openfilelist[0].date=((fcb *)(myvhard+5*BLOCKSIZE))->date;
   openfilelist[0].first=((fcb *)(myvhard+5*BLOCKSIZE))->first;
   openfilelist[0].length=((fcb *)(myvhard+5*BLOCKSIZE))->length;
   openfilelist[0].free=1;
   openfilelist[0].dirno=5;
   openfilelist[0].diroff=0;
   strcpy(openfilelist[0].dir, "/root/");
   openfilelist[0].count=0;
   openfilelist[0].fcbstate=0;
   openfilelist[0].topenfile=1;

   memset(currentdir,0,sizeof(currentdir));
   strcpy(currentdir,"/root/");
   strcpy(openfilelist->dir[0],currentdir);
   startp=((block0 *)myvhard)->startblock;
   ptrcurdir=&openfilelist[0];

 }

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


int find_fd_by_openfile(char filename[],char exname[])
{
	int i=0;
    for(i=0;i<MAXOPENFILE;i++)
    {
        if(strcmp(filename,openfilelist[i].filename)==0&&strcmp(exname,openfilelist[i].exname)==0)
           return i;
    }
   return -1;
} 


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