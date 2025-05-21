#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <setjmp.h>

#define PAGE_COUNT 10

void *region;
size_t page_size;
char *ptr;
sigjmp_buf jump_buffer;

void sigHandler(int sig)
{
    printf("SIGSEGV caught! Access violation occurred.\n");
    siglongjmp(jump_buffer, 1); 
}

int main()
{
    
    printf("PID: %d\n", getpid());
    printf("Run: cat /proc/%d/maps\n", getpid());
    sleep(10); 
    
    page_size = getpagesize();
    region = mmap(NULL, PAGE_COUNT * page_size,
                  PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (region == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    ptr = (char *)region;
    printf("Memory mapped at: %p\n", region);
    sleep(10); 
    signal(SIGSEGV, sigHandler);

    
    printf("\n--- Testing PROT_WRITE only (no read) ---\n");
    if (mprotect(region, PAGE_COUNT * page_size, PROT_WRITE) != 0) 
    {
        perror("mprotect (write only)");
        exit(EXIT_FAILURE);
    }

    
    if (sigsetjmp(jump_buffer, 1) == 0) 
    {
        printf("Trying to read (should cause SIGSEGV)...\n");
        char val = ptr[5];
        printf("Value: %c\n", val); 
    } else {
        printf("Recovered from read violation.\n");
    }

    sleep(5);

    
    printf("\n--- Testing PROT_READ only (no write) ---\n");
    if (mprotect(region, PAGE_COUNT * page_size, PROT_READ) != 0) 
    {
        perror("mprotect (read only)");
        exit(EXIT_FAILURE);
    }

    if (sigsetjmp(jump_buffer, 1) == 0) 
    {
        printf("Trying to write (should cause SIGSEGV)...\n");
        ptr[1] = 'A'; 
        printf("Written!\n"); 
    } else
    {
        printf("Recovered from write violation.\n");
    }

    sleep(10);

    
    printf("\n--- Unmapping pages 4 to 6 ---\n");
    if (munmap(ptr + 4 * page_size, 3 * page_size) != 0) 
    {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    printf("Pages 4-6 unmapped.\n");
    sleep(10); 

    printf("Done. Exiting.\n");
    return 0;
}
