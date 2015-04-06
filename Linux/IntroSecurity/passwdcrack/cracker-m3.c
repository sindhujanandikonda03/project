/* This program is trying to crack the salted SHA1 hash */

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
  time_t start, end;
  int rc = 0, c, i, k;
  FILE *fd, *dict;
  char line[100], word[100];
  char first[100];
  char *second, *third, *tmp;
  int success = 0;
  unsigned char buf[SHA_DIGEST_LENGTH];
  char hash[SHA_DIGEST_LENGTH*2];
  unsigned char salt[8];
  SHA_CTX ctx;

  time(&start);
  // read file and compare the stored hash
  fd = fopen("sha1-salted.txt", "r");
  //dict = fopen("words", "r");
  dict = fopen("/usr/share/dict/words", "r");
  
  while(!feof(fd)){
    if((fgets(line, 100, fd)) != NULL){
       strcpy(first, line);
       //Get rid of newline
       strtok_r (first, "\n", &third);
       strtok_r (first, "\t", &second);
       strtok_r(second, "\t", &third);
       //printf("%s\n", first);
       //convert to byte array
       for(i = 0; i < strlen(second) / 2; i++) {
           sscanf(second+2*i, "%02x", &k);
           salt[i] = (char)k;
           //printf("%2x", salt[i]);
        }
       //printf("\n");
       //printf("--------------------------------------\n");
       while(!feof(dict)){
         if((fgets(word, 100, dict)) != NULL){
	   strtok_r (word, "\n", &tmp);
           //printf("\n%s\n", word);
           SHA1_Init(&ctx);
           SHA1_Update(&ctx, word, strlen(word));
           SHA1_Update(&ctx, salt, 8);
           SHA1_Final(buf, &ctx);

	   for (i=0; i < SHA_DIGEST_LENGTH; i++) {
              sprintf((char*)&(hash[i*2]), "%02x", buf[i]);
           }
           //if(strcmp(word, "password") == 0)  printf("\n%s\n", hash);

           if(strcmp(third, hash) == 0){
               time(&end);
               printf("%dsec\t", (int)(end - start));
               success++;
               printf("%d\t", success);
               printf("%s\t", first);
               printf("%s\n", word);
               break;
           }
        }
      }
      rewind(dict);
   }   
 }
  fclose(fd);
  fclose(dict);
  //printf("\n%d\n", success);
  return 0;
}

