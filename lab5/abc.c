#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



int globalNotInitedInt;
int globalInitedInt = 5;
char *globalArrayChar = "Hello, World!";

const int globalConstInt = 123;
const int globalConstNotInit;

static int staticVarGlobal = 777;




void showLocalAddresses() 
{
  
  int localVarInit = 5;
  printf("local initialized: \t %p (%d)\n", &localVarInit, localVarInit);

  int localVarNotInit;
  printf("local not init: \t %p (%d)\n", &localVarNotInit, localVarNotInit);

  char *localCharArray = "Hello from local";
  printf("local char arr: \t %p (%s)\n", &localCharArray, localCharArray);

  
  static int staticInit = 1;
  printf("static int init: \t %p (%d)\n", &staticInit, staticInit);

  static int staticNotInit;
  printf("static int not init: \t %p (%d)\n", &staticNotInit, staticNotInit);

  static char *staticChar = "Hello from static char";
  printf("static char arr: \t %p 0x%p (%s)\n", &staticChar, staticChar, staticChar);

  
  const int myConst = 78;
  printf("const int: \t\t %p (%d)\n", &myConst, myConst);

  const char *constChar = "h e l l o   m y  c o n s t";
  printf("const char arr: \t %p (%s)\n", &constChar, constChar);


  
  char *myBigArray = (char*)calloc(1 * 1024 * 1024, sizeof(char));
  printf("my big arr: \t %p 0x%p (%s)\n", &myBigArray, myBigArray, myBigArray);
  
}

int main() 
{

  printf("function(): \t\t %p\n", showLocalAddresses);
  printf("========================================================\n"); 
 
  printf("LOCAL VARIABLES\n");
  showLocalAddresses();

  printf("========================================================\n");

  printf("GLOBAL VARIABLES\n");
  printf("global not inited int: \t\t %p (%d)\n", &globalNotInitedInt, globalNotInitedInt);
  printf("global inited int: \t\t %p (%d)\n", &globalInitedInt, globalInitedInt);
  printf("global array char: \t\t %p 0x%p (%s)\n", &globalArrayChar, globalArrayChar, globalArrayChar);

  printf("global const int inited: \t %p (%d)\n", &globalConstInt, globalConstInt);
  printf("global const int not inited: \t %p (%d)\n", &globalConstNotInit, globalConstNotInit);

  printf("static global int: \t %p (%d)\n", &staticVarGlobal, staticVarGlobal);

  printf("========================================================\n");
 
  printf("pid: %d\n", getpid());

  sleep(30);

  return 0;
}
