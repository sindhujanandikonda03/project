#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main (int argc, char *argv[])
{
  int rc = 0;
  int fd = -1;
  
  fd = open("etc-passwd-plain.txt", O_CREAT|O_RDWR|O_APPEND, 666);

  rc = dup2(fd, 1);

  printf("%s\t%s\n", argv[1], argv[2]);

  return 0;
}

