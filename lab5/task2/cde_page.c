#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>

#define PAGE_COUNT 10

void sigHandler(int sig)
{
    printf("SIGSEGV caught! Access violation occurred.\n");
    exit(EXIT_FAILURE);
}

int main() 
{
    
    printf("PID: %d\n", getpid());
    printf("Open another terminal and run: cat /proc/%d/maps\n", getpid());
    sleep(10); 
    size_t page_size = getpagesize();
    void *region = mmap(NULL, PAGE_COUNT * page_size,
                        PROT_NONE,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (region == MAP_FAILED)
    {
        printf("mmap failed");
        exit(EXIT_FAILURE);
    }

    printf("Memory mapped at: %p\n", region);
    sleep(15); 
    signal(SIGSEGV, sigHandler);

    printf("\n--- Testing PROT_WRITE only (no read) ---\n");
    if (mprotect(region, PAGE_COUNT * page_size, PROT_WRITE) != 0) 
    {
        printf("mprotect (write only)");
        exit(EXIT_FAILURE);
    }

    char *ptr = (char *)region;
    printf("Trying to read (should cause SIGSEGV)...\n");
    char val = ptr[0]; 
    printf("Value: %c\n", val); 
    printf("\n--- Testing PROT_READ only (no write) ---\n");
    if (mprotect(region, PAGE_COUNT * page_size, PROT_READ) != 0) {
        printf("mprotect (read only)");
        exit(EXIT_FAILURE);
    }

    printf("Trying to write (should cause SIGSEGV)...\n");
    ptr[1] = 'A'; 
    printf("Written!\n"); 
    sleep(15);
    if (munmap((char *)region + 4 * page_size, 3 * page_size) != 0) {
        printf("munmap failed");
        exit(EXIT_FAILURE);
    }

    printf("Pages 4-6 unmapped.\n");
    sleep(15);
    printf("Check memory map again.\n");

    return 0;
}
