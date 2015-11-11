#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include "header.h"
#include "print.h"
#include "parser.h"

char *path;
char cwd[1024];
int filedes;







void main()
{
  int bytes_read;
  size_t lbytes=0;
  char *command;
  path=(char *)malloc(sizeof(char)*1024);
  getcwd(cwd,sizeof(cwd));
  path = cwd;
  command=(char*)malloc(lbytes+1);
  printshell(path);
  while(1)
    {
      bytes_read=getline(&command,&lbytes,stdin);
      diffcommands(command,bytes_read);
    }
}
