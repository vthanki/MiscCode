#include <stdio.h>
#include <stdlib.h>


int main( int argc, char *argv[] )
{

  FILE *fp;
  char path[1035];
  int ret;

  /* Open the command for reading. */
  fp = popen(argv[1], "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    printf("[+] %s", path);
  }

  /* close */
  ret = pclose(fp);
  printf("[*] ret:%d\n", ret);

  return 0;
}
