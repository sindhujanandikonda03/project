#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <openssl/sha.h>
#include <openssl/rand.h>


int main (int argc, char *argv[])
{
  int rc = 0, i = 0, k, size;
  int fd = -1;
  unsigned char tmp[SHA_DIGEST_LENGTH];
  char hash[SHA_DIGEST_LENGTH*2];
  char *tst;
  char *ss = "0d24633cd1e013d3";
  unsigned char rand_salt[8];
  //unsigned char salt[8];
  SHA_CTX ctx;
  
  fd = open("etc-passwd-salted.txt", O_CREAT|O_RDWR|O_APPEND, 0666);
  
  rc = dup2(fd, 1);
  RAND_bytes(rand_salt, 8);

/*  for(i = 0; i < strlen(ss) / 2; i++) {
        sscanf(ss+2*i, "%02x", &k);
        salt[i] = (char)k;
        //printf("%2x\n", salt[i]);
  }*/

  SHA1_Init(&ctx);
  SHA1_Update(&ctx, argv[2], strlen(argv[2]));
  SHA1_Update(&ctx, rand_salt, 8);
  
  SHA1_Final(tmp, &ctx);

  for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(hash[i*2]), "%02x", tmp[i]);
  }

  printf("%s\t", argv[1]);
  for(i=0;i<8;i++)
    printf("%02x", rand_salt[i]);
  printf("\t%s\n", hash);

  return 0;
}

