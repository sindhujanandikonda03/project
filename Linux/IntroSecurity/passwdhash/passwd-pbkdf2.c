#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <openssl/rand.h>


int main (int argc, char *argv[])
{
  int rc = 0, i = 0, k, size;
  int fd = -1;
  unsigned char tmp[32];
  char hash[32*2];
  char *tst;
  char *ss = "964b6ab62ac547ab";
  unsigned char rand_salt[8];
  //unsigned char salt[8];
  
  fd = open("etc-passwd-pbkdf2.txt", O_CREAT|O_RDWR|O_APPEND, 0666);
  
  rc = dup2(fd, 1);
  RAND_bytes(rand_salt, 8);

 /* for(i = 0; i < strlen(ss) / 2; i++) {
        sscanf(ss+2*i, "%02x", &k);
        salt[i] = (char)k;
        printf("%2x\n", salt[i]);
  }*/

  PKCS5_PBKDF2_HMAC_SHA1(argv[2], strlen(argv[2]), rand_salt, strlen(rand_salt), 10000, 32, tmp);

  for (i=0; i < 32; i++) {
        sprintf((char*)&(hash[i*2]), "%02x", tmp[i]);
  }

  printf("%s\t", argv[1]);
  for(i=0;i<8;i++)
    printf("%02x", rand_salt[i]);
  printf("\t%s\n", hash);

  return 0;
}

