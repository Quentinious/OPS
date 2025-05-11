#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv, char **envp) 
{
  if (argc == 3 && setenv(argv[1], argv[2], 1) != 0)
  {
    printf("setenv()");
    return 0;
  }

  printf("env var's value: %s\n", getenv(argv[1]));

  if (setenv(argv[1], "5", 1) != 0) 
  {
    printf("setenv()");
    exit(EXIT_FAILURE);
  }
  printf("new env var's value: %s\n", getenv(argv[1]));
  return 0;
  
}

