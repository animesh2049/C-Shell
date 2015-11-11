#include "header.h"

void printshell(char *path)
{
  char *host,*user;
  host=(char*)malloc(10*sizeof(char));
  user=(char*)malloc(10*sizeof(char));
  getlogin_r(user,10);
  gethostname(host, 10);
  printf("<%s@%s:%s>",user,host,path);
}
