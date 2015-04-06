#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <openssl/sha.h>

int main (int argc, char *argv[])
{
  int rc = 0, i = 0;
  int fd = -1;
  unsigned char tmp[SHA_DIGEST_LENGTH];
  char hash[SHA_DIGEST_LENGTH*2];

  fd = open("etc-passwd-hashed.txt", O_CREAT|O_RDWR|O_APPEND, 0666);

  rc = dup2(fd, 1);
  
  SHA1((unsigned char *)argv[2], strlen(argv[2]), tmp);
  
  for (i=0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf((char*)&(hash[i*2]), "%02x", tmp[i]);
    }


  printf("%s\t%s\n", argv[1], hash);

  return 0;
}

