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
  int rc = 0, c, i, k;
  FILE *fd;
  char line[100];
  char first[100];
  char *second, *third;
  int success = 0;
  unsigned char buf[SHA_DIGEST_LENGTH];
  char hash[SHA_DIGEST_LENGTH*2];
  unsigned char salt[8];
  SHA_CTX ctx;

  // read file and compare the stored hash
  fd = fopen("sha1-salted.txt", "r");
  
  while(!feof(fd)){
    if((fgets(line, 100, fd)) != NULL){

       strcpy(first, line);
       //Get rid of newline
       strtok_r (first, "\n", &third);
       strtok_r (first, "\t", &second);
       strtok_r(second, "\t", &third);
       //printf("%s\n", second);

       if(strcmp(first, argv[1]) == 0){
        //convert to byte array
        for(i = 0; i < strlen(second) / 2; i++) {
            sscanf(second+2*i, "%02x", &k);
            salt[i] = (char)k;
            //printf("%2x\n", salt[i]);
        }
        // hash password      
        SHA1_Init(&ctx);
        SHA1_Update(&ctx, argv[2], strlen(argv[2]));
        SHA1_Update(&ctx, salt, 8);  
        SHA1_Final(buf, &ctx);
  
        for (i=0; i < SHA_DIGEST_LENGTH; i++) {
           sprintf((char*)&(hash[i*2]), "%02x", buf[i]);
        }

        //printf("%s\n", hash);

        if(strcmp(third, hash) == 0){
               success = 1;
               break;
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

