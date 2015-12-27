
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



