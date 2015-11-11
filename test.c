#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>

char **commandargs(char *input, char **command);
void printshell();
void diffcommands(char *line,int size);
void executecommand(char **command);

void printshell(char *path)
{
  char *host,*user;
  host=(char*)malloc(10*sizeof(char));
  user=(char*)malloc(10*sizeof(char));
  getlogin_r(user,10);
  gethostname(host, 10);
  printf("<%s@%s:%s>",user,host,path);
}

void diffcommands(char *line,int size)
{
  int i=0,j=0;
  char **token,**commandtoken,**args,*demo;
  char s[1]=";";
  token=(char **)malloc(sizeof(char*)*(size+1));
  token[i]=demo=strtok(line, s);
  i++;
  while(demo != NULL)
    {
      demo=strtok(NULL,s);
      token[i]=demo;
      i++;
    }
  int number_of_commands=0;
  while(number_of_commands < i-1)
    {
      j=0;
      int size = (unsigned)strlen(token[number_of_commands]);
      commandtoken=(char **)malloc(sizeof(char *)*size);
      demo=strtok(token[number_of_commands]," \t\n\0");
      commandtoken[j]=demo;
      j++;
      while(demo != NULL)
	{
	  demo=strtok(NULL," \t\n\0");
	  commandtoken[j]=demo;
	  j++;
	}
      commandtoken[j]='\0';
      if(strcmp(commandtoken[0],"exit") == 0)
	{
	  exit(0);
	}
      else if(strcmp(commandtoken[0],"cd\0")==0)
	{
	  if(commandtoken[1] != NULL)
	    {
	      if(chdir(commandtoken[1]) != 0)
		{
		  perror("Sorry can't execute");
		}
	    }
	}
      else
	{
	  executecommand(commandtoken);
	}
      number_of_commands++;
    }
}

void executecommand(char **command)
{
  pid_t pid;
  int status;
  pid = fork();
  if(pid < 0)
    {
      printf("Sorry Can't execute command since pid < 0\n");
      exit(1);
    }
  else if(pid == 0)
    {
      if(execvp(*command,command) < 0)
	{
	  printf("Sorry Can't execute command since execvp < 0\n");
	  exit(1);
	}
    }
  else
    {
      while(wait(&status) != pid);
    }
}

void main()
{
  int bytes_read;
  size_t lbytes=0;
  char *command;
  char *path;
  path=(char *)malloc(sizeof(char)*255);
  *path = '~';
  command=(char*)malloc(lbytes+1);
  while(1)
    {
      printshell(path);
      bytes_read=getline(&command,&lbytes,stdin);
      diffcommands(command,bytes_read);
    }
}
