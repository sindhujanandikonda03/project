#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "p2.h"

int main(int argc, char* argv[])
{
  char final[20]="";
  char temp[10]="";
  strncpy(final, STR1+1, 4);
  strncat(final, STR2+1, 4);
  strncpy(temp, STR1, 5);
  //printf("%s\n", temp);
  if(strncmp(argv[0]+2, final, strlen(final))){
    return unlink(argv[0]);
  }
  else{
    if(strncmp(temp, argv[1], strlen(argv[1]))){
       printf("Password Incorrect! Try Aagin.\n");
    }
    else
       printf("Good Job!!.\n");
  }
}
