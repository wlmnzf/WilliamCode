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
  int i=0;
  for(i=0;i<MAXOPENFILE;i++)
  {
     if(openfilelist[i].topenfile==0)
         my_close(i);
  }
  free(myvhard);
}
