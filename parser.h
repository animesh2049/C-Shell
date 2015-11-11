#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include "header.h"
#include "execute.h"



void diffcommands(char *line,int size)
{
  int i=0,j=0,size_token,k=0;
  int l;
  int number_of_commands=0;
  char **token,**commandtoken,**args,*demo,*demo2,**tokenwithpipe;
  char pipe[1]="|";
  char s[1]=";";
  token=(char **)malloc(sizeof(char*)*(size+1));
  token[i]=demo=strtok(line, s);
  i++;
  while(demo != NULL) /* for parsing of semicolon */
    {
      demo=strtok(NULL,s);
      token[i++]=demo;
    }
  for(l=0;l<2;l++)
    printf("token before semicolon is:%s\n",token[l]);
  printf("\n");
  while(number_of_commands < i-1)  /* for parsing of pipe */
    {
      size_token = (unsigned) strlen(token[number_of_commands]+1);
      tokenwithpipe = (char **)malloc(sizeof(char *)*size_token);
      demo=strtok(token[number_of_commands++],pipe);
      printf("demo is:%s\n",demo);
      tokenwithpipe[k++]=demo;
      /* printf("tokenwithpipe is:%s k is:%d\n",tokenwithpipe[k-1],k-1); */
      printf("testing %s\n",tokenwithpipe[0]);
      while(demo != NULL)
	{
	  demo=strtok(NULL,pipe);
	  printf("demo is:%s\n",demo);
	  if(demo != NULL)
	    tokenwithpipe[k++]=demo;
	  printf("testing %s\n",tokenwithpipe[0]);
	  /* printf("tokenwithpipe is:%s k is:%d\n",tokenwithpipe[k-1],k-1); */
	}
    }
  printf("testing %s\n",tokenwithpipe[0]);
  for(l=0;l<2;l++)
    printf("going to parse %s l is:%d\n",tokenwithpipe[l],l);
  printf("\n");
  number_of_commands = 0;
  while(number_of_commands < k-1)    /* for parsing of spaces */
    {
      int flag;
      char *pathappen = path;
      j=0;
      char *filename;
      printf("to be parsed:%s\n",tokenwithpipe[number_of_commands]);
      int size = (unsigned)strlen(tokenwithpipe[number_of_commands]);
      commandtoken=(char **)malloc(sizeof(char *)*size);
      demo=strtok(tokenwithpipe[number_of_commands]," \t\n\0");
      printf("demo is:%s\n",demo);
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
	  if(chdir(commandtoken[1]) != 0)
	    {
	      perror("Sorry can't execute");
	    }
	  else
	    {
	      if(strcmp(commandtoken[1],"..")==0)
		{
		  char **path_build;
		  path_build = (char **)malloc(sizeof(char*)*(strlen(path)));
		  demo = strtok(path,"/");
		  path_build[0]=demo;
		  demo2 = demo;
		  int l=0;
		  while(demo != NULL)
		    {
		      demo = demo2;
		      demo2=strtok(NULL,"/");
		      if(demo2 != NULL)
			path_build[l]=demo;
		    }
		  path = *path_build;
		  printshell(path);
		}
	      else if(strcmp(commandtoken[1],".")==0)
		{
		  printshell(path);
		}
	      else
		{
		  while(*path != '\0')
		    {
		      path++;
		    }
		  *path='/';
		  path++;
		  int k=0;
		  while(commandtoken[1][k] != '\0')
		    {
		      *path=commandtoken[1][k];
		      k++;
		      path++;
		    }
		  path = pathappen;
		  printshell(pathappen);
		}
	    }
	}
      else if(strcmp(commandtoken[0],"echo")==0)
	{
	  int z=1,args=1;
	  while(z < j-1)
	    {
	      char *echoargs = (char*)malloc(sizeof(strlen(commandtoken[z])));
	      echoargs = strtok(commandtoken[z],"\" ");
	      if(echoargs != NULL && (strcmp(echoargs,">")) != 0 && (strcmp(echoargs,">>")) != 0)
		{
		  commandtoken[args]=echoargs;
		  args++;
		}
	      else if(strcmp(echoargs,">")== 0)
		{
		  flag = 1;
		  commandtoken[args]="";
		  args++;
		  z++;
		  filename = strtok(commandtoken[z],"\" ");
		  commandtoken[args]="";
		  args++;
		}
	      else if(strcmp(echoargs,">>")==0)
		{
		  flag=2;
		  commandtoken[args]="";
		  args++;
		  z++;
		  filename = strtok(commandtoken[z],"\" ");
		  commandtoken[args++]="";
		}
	      else if(strcmp(echoargs,"<") == 0)
		{
		  flag=3;
		  commandtoken[args++] = "";
		  z++;
		  filename = strtok(commandtoken[z],"\" ");
		  commandtoken[args++]="";
		}
	      z++;
	    }
	  executecommand(commandtoken,flag,filename);
	}
      else
	{
	  int v=0;
	  while(commandtoken[v])
	    {
	      //    if()
	      v++;
	    }
	  executecommand(commandtoken,0,filename);
	}
      number_of_commands++;
    }
}
