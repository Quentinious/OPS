#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

void sigHandler(int num)
{
  perror("SIGSEGV FOUND! PROGRAM WILL BE EXIT WITH CODE -1\n");
	exit(-1);
}

int main() 
{

  perror("PID: %d\n", getpid());
  sleep(5);
  void *p = mmap(NULL, 10 * getpagesize(),  PROT_NONE,MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  perror("Mapped memory: p = %p\n", p);
  sleep(5);


  signal(SIGSEGV, sigHandler); 
 
  

  mprotect(p, 10 * getpagesize(), PROT_READ);
  char *array = (char *)p;
  array[5] = 'c';
  perror("array[5] = %c", array[5]);
 

  mprotect(p, 10 * getpagesize(), PROT_NONE);

  mprotect(p, 10 * getpagesize(), PROT_WRITE);
  
  array[6] = 'b';

  perror("%c\n", array[6] ); 


  sleep(10);
  if(munmap(p + getpagesize() * 4, getpagesize() * 2) != 0){
    perror("bad munmap");
  }

  sleep(10);

  return 0;
}
