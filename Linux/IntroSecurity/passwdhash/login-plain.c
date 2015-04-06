#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main (int argc, char *argv[])
{
  int rc = 0, c;
  FILE *fd;
  char line[100];
  char first[100];
  char *second, *tmp;
  int success = 0;

  fd = fopen("etc-passwd-plain.txt", "r");
  
  while(!feof(fd)){
    if((fgets(line, 100, fd)) != NULL){

       strcpy(first, line);
       //Get rid of newline
       strtok_r (first, "\n", &tmp);
       strtok_r (first, "\t", &second);
       if(strcmp(first, argv[1]) == 0){
          if(strcmp(second, argv[2]) == 0){
               success = 1;
          }
       }
       else
          continue;
   }   
 }
 
  if(success == 1)  printf("SUCCESS\n");
  else              printf("ACCESS DENIED\n");

  fclose(fd);

  return 0;
}

