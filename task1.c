#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int global_var = 100;

int main() 
{

    int local_var = 200;

    printf("=== [PARENT] ===\n");
    printf("PID: %d\n", getpid());
    printf("Address of global_var: %p, value: %d\n", (void*)&global_var, global_var);
    printf("Address of local_var:  %p, value: %d\n", (void*)&local_var, local_var);

    pid_t pid = fork();

    if (pid == -1) 
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {

        printf("\n=== [CHILD] ===\n");
        printf("PID: %d, PPID: %d\n", getpid(), getppid());
        printf("Address of global_var: %p, value: %d\n", (void*)&global_var, global_var);
        printf("Address of local_var:  %p, value: %d\n", (void*)&local_var, local_var);


        global_var = 500;
        local_var = 600;

        printf("Changed global_var to %d\n", global_var);
        printf("Changed local_var to %d\n", local_var);

        exit(5);
    }
    else
    {
        sleep(30);
        printf("\n=== [PARENT after CHILD] ===\n");
        printf("Global var: %d\n", global_var);
        printf("Local var: %d\n", local_var);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) 
        {
            printf("Child exited normally with code %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) 
        {
            printf("Child terminated by signal %d\n", WTERMSIG(status));
        } else 
        {
            printf("Child exited abnormally\n");
        }
    }

    return 0;
}
