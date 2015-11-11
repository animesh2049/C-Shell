#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include "header.h"



void executecommand(char **command,int flag,char *filename)
{
  pid_t pid;
  int status;
  pid = fork();
  if(pid < 0)
    {
      perror("Sorry Can't execute command since pid < 0\n");
      exit(1);
    }
  else if(pid == 0)
    {
      if(flag == 1)         /* output redirection for overwrite for echo */
	{
	  close(STDOUT_FILENO);
	  open(filename,O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
	}
      else if(flag == 2)   /* output redirection for append for echo*/
	{
	  close(STDOUT_FILENO);
	  open(filename,O_CREAT|O_APPEND|O_WRONLY,S_IRWXU);
	}
      else if(flag == 3)     /* input redirection for echo */
	{
	  int i;
	  while(command[i++])
	    printf("%s ",command[i]);
	  printf("\n");
	  close(STDIN_FILENO);
	  open(filename,O_RDONLY);
	}
      else if(flag == 4)
	{
	  
	}
      if(execvp(*command,command) < 0)
	{
	  printf("Sorry Can't execute command since execvp < 0\n");
	  exit(1);
	}
      printshell(path);
    }
  else
    {
      while(wait(&status) != pid);
      printshell(path);
    }
}
