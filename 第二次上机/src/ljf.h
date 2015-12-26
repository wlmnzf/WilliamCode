int my_open(char *filename)
{ 
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
  		if (strcmp(openfilelist[i].filename, fname) == 0 && strcmp(openfilelist[i].exname, exname) == 0 && i != curfd)
			{
			printf("出错,该文件已经打开.\n");
			return -1;
		}
	}
	openfilelist[curfd].count = 0;
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
	openfilelist[fd].dirno = openfilelist[curfd].first;
	openfilelist[fd].diroff = i;

	strcpy(openfilelist[fd].dir[fd], openfilelist[curfd].dir[curfd]);
	strcat(openfilelist[fd].dir[fd], filename);
	if (fcbptr->attribute & 0)
	strcat(openfilelist[fd].dir[fd], "\\");
	openfilelist[fd].father = curfd;
	openfilelist[fd].count = 0;
	openfilelist[fd].fcbstate = 0; 
	openfilelist[fd].topenfile = 1; 
	printf("(directory/file is opened successfully)\n");
	return fd; 
}


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
		if (curfd)
		curfd = my_close(curfd); 
		
		cmd_head[100]
		return;
	}

	else if (strcmp(dir, "root") == 0)
	{
		while (curfd)
		curfd = my_close(curfd);  
		dir = strtok(NULL, "\\");  
		strcpy(cmd_head,"root>");
	}
	while (dir)
	{
		fd = my_open(dir); 
		if (fd != -1) 
		{
			curfd = fd;  
			strcat(cmd_head,dir);
			strcat(cmd_head,">");
		}
		else
			return;
		dir = strtok(NULL, "\\");
	}
   printf("enter this contents now\n");
}
