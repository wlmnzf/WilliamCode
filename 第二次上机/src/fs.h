#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

#define  BLOCKSIZE  1024
#define  SIZE         1024000
#define  END         65535
#define  FREE        0
#define  ROOTBLOCKNUM  2
#define  MAXOPENFILE    10

#define  MAX_OPEN_FILE    10//
#define MAX_TXT_SIZE 1000 //
#define MAXTEXT 1000

typedef struct FCB
{
  char filename[8];
  char exname[3];
  unsigned char attribute;
  unsigned short time;
  unsigned short date;
  unsigned short first;
  unsigned long length;
  char free;
}fcb;

typedef struct FAT
{
		unsigned short id;
}fat;

typedef struct USEROPEN
{
		char filename[8];
		char exname[3];
		unsigned char attribute;
		unsigned short time;
		unsigned short date;
		unsigned short first;
		unsigned long length;
    char free;
    int dirno;
    int diroff;
    char dir[MAXOPENFILE][80];
	int count;
    char fcbstate;
    char topenfile;

    int father;
}useropen;


int curfd;

typedef struct BLOCK0
{
    char magic[9];
    char information[200];
    unsigned short root;
    unsigned char *startblock;
}block0;


unsigned char *myvhard;
char magicnum[9]="10101010";
char myfsys[100]="myfsys";
char filename[100]="myfsys";//
char default_exname[20]="";
useropen openfilelist[MAXOPENFILE];
useropen *ptrcurdir;
char currentdir[80];
unsigned char* startp;

void startsys();
void my_format();
void my_cd(char *dirname);
void my_mkdir(char *dirname);
void my_rmdir(char *dirname);
void my_ls();
int my_create(char *filename);
void my_rm(char *filename);
int my_open(char *filename);
int my_close(int fd);
int my_write(int fd);
int do_write(int fd,char *text,int len,char wstyle);
unsigned short findFree();
int my_read(int fd,int len);
int do_read(int fd,int len,char *text);
void my_exitsys();
int findFreeO();
unsigned short findblock();
unsigned short findFree();


char cmd_head[100]="root>";
char cmd[100];
//unsigned short findblock();



//int purcurdir;///
//int curdir;///
