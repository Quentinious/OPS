#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(void) 
{
  const int sizeBuffer = 100;
  // i
  char *buffer = (char* )calloc(sizeBuffer, sizeof(char));
  if(!buffer) 
  {
    return 0;
  }
  strcpy(buffer, "Hello, Wrold");
  printf("%s\n", buffer);
  free(buffer);
  printf("%s\n", buffer); 
  char *newBuffer = (char*)calloc(sizeBuffer, sizeof(char));
  strcpy(newBuffer, "Another Hello World!");
  printf("%s\n", newBuffer); 

  
  char *pointer = &newBuffer[sizeBuffer / 2];

  
  free(&newBuffer[sizeBuffer / 2]);
  
  printf("%s\n", newBuffer);

  return 0;
}

