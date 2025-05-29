#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12346
#define BUFFER_SIZE 1024



void handle_client(int client_sock)
{
    char buffer[BUFFER_SIZE];
    ssize_t n;

    printf("Обработка клиента в процессе %d\n", getpid());

    while ((n = read(client_sock, buffer, BUFFER_SIZE)) > 0)
    {
        write(client_sock, buffer, n);
    }

    printf("Клиент отключился. Закрытие соединения в процессе %d\n", getpid());
    close(client_sock);
    exit(0);
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

    
    signal(SIGCHLD, SIG_IGN);

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }


    if (listen(server_sock, 5) < 0)
    {
        perror("listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("TCP эхо-сервер запущен на порту %d\n", PORT);

    while (1)
    {
      

        socklen_t len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &len);

        if (client_sock < 0)
        {
            perror("accept failed");
            continue;
        }
        printf("Новое соединение от %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        pid_t pid = fork();
        if (pid == 0)
        {
            close(server_sock); 
            handle_client(client_sock);
        }
        else if (pid > 0)
        {
            close(client_sock); 
        }
        else
        {
            perror("fork failed");
        }
    }

    close(server_sock);
    return 0;
}

