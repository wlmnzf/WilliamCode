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
	
    openfilelist[curfd].count = 0;
	
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
		if(strcmp(fcbptr->filename,"")==0)
			break;
		fcbptr++;
	} 
    //openfilelist[curfd].count=i*sizeof(fcb);
    fd=findopenfile();
	if(fd<0)
		return; 

    blkno=findblock();		
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
	fcbptr->length = 2 * sizeof(fcb);
	fcbptr->free = 1;							
	openfilelist[curfd].count = i * sizeof(fcb);	
	do_write(curfd, (char *)fcbptr, sizeof(fcb), 2);

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
