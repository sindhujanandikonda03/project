/* This program is trying to crack SHA1 hash of the words with punctuations */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <openssl/sha.h>

int hash_check(char words[], char *store_hash){
  unsigned char sha_buf[SHA_DIGEST_LENGTH];
  char sha[SHA_DIGEST_LENGTH*2];
  int i;

  SHA1((unsigned char *)words, strlen(words), sha_buf);
  for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(sha[i*2]), "%02x", sha_buf[i]);
  }
  if(strcmp(words, "orihan") == 0)  
      printf("%s\n", sha);
  if(strcmp(sha, store_hash) == 0)  return 0;
  else return 1;  
}

int main ()
{
  time_t start, end;
  int m, i, k, punctuation_set_size;
  FILE *fd, *dict, *punctuation;
  char line[100], word[30];
  char punc[50] = {'0','1','2','3','4','5','6','7','8','9'};
  char first[100];
  char *second, *tmp;
  int success = 0;
  unsigned char buf[SHA_DIGEST_LENGTH];
  char hash[SHA_DIGEST_LENGTH*2];
  char words_punc[30];
  char pass[30];
  int  cnt = 0;

  time(&start);
  // read file
  fd = fopen("etc-passwd-m2.txt", "r");
  dict = fopen("words", "r");
  //dict = fopen("/usr/share/dict/words", "r");
  punctuation = fopen("punctuation.txt", "r");
  
  // retrieve punctuation char
  while(!feof(punctuation)){
        if((fgets(word, 100, punctuation)) != NULL){
	   strtok_r (word, "\n", &tmp);
        }
  }
  
  fclose(punctuation);

  strcat(punc, word);
  //printf("%s\n", punc);
  punctuation_set_size = strlen(punc);
  while(!feof(fd)){
    if((fgets(line, 100, fd)) != NULL){

       strcpy(first, line);
       //Get rid of newline
       strtok_r (first, "\n", &tmp);
       strtok_r (first, "\t", &second);
       success = 0;
       //check dictionary words
       while(!feof(dict)){
            if((fgets(word, 100, dict)) != NULL){
               strtok_r (word, "\n", &tmp);
               //printf("%s\n", word);
               if(hash_check(word, second) == 0){
                     success = 1;
                     strcpy(pass, word);
                     //printf("%s", pass);
                         break;
               }
            }
       }
       rewind(dict);
       //check non-dictionary words
       if(success == 0){
         //check *pass or pass*
         while(!feof(dict)){
            if((fgets(word, 100, dict)) != NULL){
               strtok_r (word, "\n", &tmp);
               for(m=0; m<=punctuation_set_size - 1; m++){
                  sprintf(words_punc, "%c%s", punc[m], word);
                  //printf("%s\n", words_punc);
                
                  if(hash_check(words_punc, second) == 0){
                     success = 1;
                     strcpy(pass, words_punc);
                         break;
                  }		
                  sprintf(words_punc, "%s%c", word, punc[m]);
                  //printf("%s\n", words_punc);
		
                  if(hash_check(words_punc, second) == 0){
                     success = 1;
		     strcpy(pass, words_punc);
                         break;
                  }
               }
	    }
          }   
       }
       rewind(dict);
       if(success == 0){
          while(!feof(dict)){
            if((fgets(word, 100, dict)) != NULL){
               strtok_r (word, "\n", &tmp);
               for(m=0; m<=punctuation_set_size - 1; m++){
                   for(k=0; k<=punctuation_set_size - 1; k++){
		       //printf("%c\n", punc[m]);
                       sprintf(words_punc, "%c%s%c", punc[m], word, punc[k]);
                       //printf("%s\n", words_punc);
                       
                       if(hash_check(words_punc, second) == 0){
                          success = 1;
                          strcpy(pass, words_punc);
                          break;
                       }                       
                   }                  
                  if(success==1) break;
               }
            }
            if(success==1) break;
          }
       }

      if(success == 1){
         time(&end);
         printf("%dsec\t", (int)(end - start));
         cnt++;
         printf("%d\t", cnt);
         printf("%s\t", first);
         printf("%s\n", pass);        
      }
      rewind(dict); 
    } 
 }
 
  fclose(fd);
  fclose(dict);

  return 0;
}

