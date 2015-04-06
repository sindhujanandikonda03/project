#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <openssl/sha.h>

int main (int argc, char *argv[])
{
  int rc = 0, c, i;
  FILE *fd;
  char line[100];
  char first[100];
  char *second, *tmp;
  int success = 0;
  unsigned char buf[SHA_DIGEST_LENGTH];
  char hash[SHA_DIGEST_LENGTH*2];
  
  // hash password
  SHA1((unsigned char *)argv[2], strlen(argv[2]), buf);
  
  for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(hash[i*2]), "%02x", buf[i]);
  }

  // read file and compare the stored hash
  fd = fopen("etc-passwd-hashed.txt", "r");
  
  while(!feof(fd)){
    if((fgets(line, 100, fd)) != NULL){

       strcpy(first, line);
       //Get rid of newline
       strtok_r (first, "\n", &tmp);
       strtok_r (first, "\t", &second);
       if(strcmp(first, argv[1]) == 0){
          if(strcmp(second, hash) == 0){
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

