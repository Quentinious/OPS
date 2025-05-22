#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    pid_t pid = fork();

    if (pid == 0)
     {
        sleep(7);  
        printf("[CHILD] My parent is: %d\n", getppid());
        exit(5);
    }
    else
    {
        printf("[PARENT] I'm going to die now. PID: %d\n", getpid());
        exit(5);
    }
}
