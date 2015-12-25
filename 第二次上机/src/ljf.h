
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
		//if (strcmp(openfilelist[i].filename, fname) == 0 && strcmp(openfilelist[i].exname, exname) == 0 && i != ptrcurdir)
  		if (strcmp(openfilelist[i].filename, fname) == 0 && strcmp(openfilelist[i].exname, exname) == 0 && i != curdir)
			{
			printf("出错,该文件已经打开.\n");
			return -1;
		}
	}
	//openfilelist[ptrcurdir].count = 0;
	openfilelist[curdir].count = 0;
//	rbn = do_read(ptrcurdir, openfilelist[ptrcurdir].length, text);
	rbn = do_read(curdir, openfilelist[curdir].length, text);
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
//	openfilelist[fd].dirno = openfilelist[ptrcurdir].first;
	openfilelist[fd].dirno = openfilelist[curdir].first;
	openfilelist[fd].diroff = i;

	//strcpy(openfilelist[fd].dir, openfilelist[ptrcurdir].dir);
	strcpy(openfilelist[fd].dir[fd], openfilelist[curdir].dir[curdir]);
	//strcat(openfilelist[fd].dir, filename);
	strcat(openfilelist[fd].dir[fd], filename);
	if (fcbptr->attribute & 0)
	//strcat(openfilelist[fd].dir, "\\");
	strcat(openfilelist[fd].dir[fd], "\\");
	//openfilelist[fd].father = ptrcurdir;
	openfilelist[fd].father = curdir;
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
if (ptrcurdir)
			ptrcurdir = my_close(ptrcurdir);  //关闭当前路径,返回值为父路径在打开表的位置; 
		return;*/
		if (curdir)
			curdir = my_close(curdir); 
		return;
	}

	else if (strcmp(dir, "root") == 0)
	{
	/*
	while (ptrcurdir)
			ptrcurdir = my_close(ptrcurdir);  //循环关闭root下的打开文件； 
		dir = strtok(NULL, "\\");  //剩下的路径；*/
		while (curdir)
			curdir = my_close(curdir);  
		dir = strtok(NULL, "\\");  
	}
	while (dir)
	{
		fd = my_open(dir); 
		if (fd != -1) 
			//ptrcurdir = fd;   //打开表项序号； 
			curdir = fd;  
		else
			return;
		dir = strtok(NULL, "\\");
	}
//	printf("目录跳转完成\n");
   printf("enter this contents now\n");
}
