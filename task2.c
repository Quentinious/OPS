#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    pid_t pid = fork();

    if (pid == -1) 
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) 
    {
        
        printf("[CHILD] PID: %d exiting...\n", getpid());
        exit(5);
    } else
    {  
        printf("[PARENT] PID: %d created child %d\n", getpid(), pid);
        sleep(30);
        printf("[PARENT] Finished sleeping. Child is now a zombie.\n");
    }

    return 0;
}
