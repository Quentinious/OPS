#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char **argv)
{

  printf("Pid: %d\n", getpid());
  sleep(1);
  if(execvp(argv[0], argv) != 0)
    {
    printf("execvp()");
    return 0;
  }

  printf("Hello, world!\n");


  return 0;
}
