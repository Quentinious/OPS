#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
 {
    pid_t pid_b = fork();
    if (pid_b == 0) 
    {
        pid_t pid_c = fork();

        if (pid_c == 0) 
        {
            printf("C (PID=%d): %d\n", getpid(), getppid());
            sleep(20);
            printf("C (PID=%d): %d\n", getpid(), getppid());
            exit(0);
        } else 
        {        
            printf("cat /proc/%d/status | grep State\n", getpid());
            sleep(10);
            printf("B (PID=%d):становлюсь зомби для A\n", getpid());
            exit(0);
        }
    }
    else 
    {
        sleep(15);
        printf("A (PID=%d):B процесс завершен\n", getpid());
        exit(0);
    }
}
