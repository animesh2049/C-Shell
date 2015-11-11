#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>

char *path;
char cwd[1024];

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
  char **token,**commandtoken,**args,*demo,*demo2;
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
      char *pathappen = path;
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
	  size_t filename;
	  int z=1,args=1;
	  while(z < j-1)
	    {
	      char *echoargs = (char*)malloc(sizeof(strlen(commandtoken[z])));
	      echoargs = strtok(commandtoken[z],"\" ");
	      if(echoargs != NULL && (strcmp(echoargs,">")) != 0)
		{
		  commandtoken[args]=echoargs;
		  args++;
		}
	      else if(strcmp(echoargs,">")== 0)
		{
		  commandtoken[args]="";
		  args++;
		  z++;
		  echoargs = strtok(commandtoken[z],"\" ");
		  commandtoken[args]="";
		  args++;
		  close(STDOUT_FILENO);
		  filename = open(echoargs,O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
		}
	      z++;
	    }
	  executecommand(commandtoken);
	  close(filename);
	}
      else
	executecommand(commandtoken);
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
      perror("Sorry Can't execute command since pid < 0\n");
      exit(1);
    }
  else if(pid == 0)
    {
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

void main()
{
  int bytes_read;
  size_t lbytes=0;
  char *command;
  path=(char *)malloc(sizeof(char)*1024);
  //  *path = '~';
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
