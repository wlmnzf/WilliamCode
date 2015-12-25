
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
