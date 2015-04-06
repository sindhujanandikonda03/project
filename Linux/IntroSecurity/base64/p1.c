#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "p1.h"

#define TABLELEN        64
#define BUFFFERLEN      128

#define ENCODERLEN      4
#define ENCODERBLOCKLEN 3

#define PADDINGCHAR     '='
#define BASE64CHARSET   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
                        "abcdefghijklmnopqrstuvwxyz"\
                        "0123456789"\
                        "+/";

int decodeblock(char *input, char *output, int index)
{
  char decodedstr[ENCODERLEN + 1] = "";

  decodedstr[0] = input[0] << 2 | input[1] >> 4;
  decodedstr[1] = input[1] << 4 | input[2] >> 2;
  decodedstr[2] = input[2] << 6 | input[3] >> 0;

  memcpy(output + index, decodedstr, ENCODERBLOCKLEN);

  return ENCODERBLOCKLEN;
}

int decode(char *input, char *output, int oplen)
{
  int length = 0;
  char *charval = 0;
  char decoderinput[ENCODERLEN + 1] = "";
  int index = 0, asciival = 0, computeval = 0, iplen = 0;
  char encodingtable[TABLELEN + 1] = BASE64CHARSET;

  iplen = strlen(input);
  while (index < iplen)
  {
    asciival = (int)input[index];
    if (asciival == PADDINGCHAR)
    {
      length += decodeblock(decoderinput, output, length);
      break;
    }
    else
    {
      charval = strchr(encodingtable, asciival);
      if (charval)
      {
        decoderinput[computeval] = charval - encodingtable;
        computeval = (computeval + 1) % 4;
        if(computeval == 0)
        {
          length += decodeblock(decoderinput, output, length);
          decoderinput[0] = decoderinput[1] = decoderinput[2] = decoderinput[3] = 0;
        }
      }
    }
    index++;
  }

  return length;
}

int main(int argc, char* argv[]) {

   char decodedoutput[BUFFFERLEN + 1] = "";

   if(argc != 2){
      printf("Usage: ./p1 <argument 1>\n");
      exit(1);
   }
   else{
      //printf("%s", argv[1]);
      decode(argv[1], decodedoutput, BUFFFERLEN);
      //printf("%s\n", decodedoutput);
      //printf("%s", USERDEF);
      if(!strncmp(decodedoutput, USERDEF, strlen(USERDEF))){
	 printf("Good Job!\n");
      }
      else{
	 printf("Sorry.Please try again!\n");
      }
   }
   return 0;
}

