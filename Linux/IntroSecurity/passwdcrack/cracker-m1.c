/* This program is trying to crack the SHA1 hash of simple words in the wordlist */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <openssl/sha.h>

int main ()
{
  time_t start, end;
  int c, i;
  FILE *fd, *dict;
  char line[100], word[100];
  char first[100];
  char *second, *tmp;
  int success = 0;
  unsigned char buf[SHA_DIGEST_LENGTH];
  char hash[SHA_DIGEST_LENGTH*2];
  
  time(&start);
  // read file and compare the stored hash
  fd = fopen("etc-passwd-m2.txt", "r");
  dict = fopen("words", "r");

  while(!feof(fd)){
    if((fgets(line, 100, fd)) != NULL){

       strcpy(first, line);
       //Get rid of newline
       strtok_r (first, "\n", &tmp);
       strtok_r (first, "\t", &second);
       while(!feof(dict)){
         if((fgets(word, 100, dict)) != NULL){
	   strtok_r (word, "\n", &tmp);
           //printf("%s\n", word);
	   // hash password
           SHA1((unsigned char *)word, strlen(word), buf);
  
           for (i=0; i < SHA_DIGEST_LENGTH; i++) {
               sprintf((char*)&(hash[i*2]), "%02x", buf[i]);
           }
           //printf("%s\n", hash);
           if(strcmp(hash, second) == 0){
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
  //printf("\n\n%d success", success);
  return 0;

}

