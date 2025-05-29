
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12346
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Подключено к серверу %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1)
    {
        printf("Введите сообщение: ");
        if (!fgets(buffer, BUFFER_SIZE, stdin))
            break;
        write(sockfd, buffer, strlen(buffer)); 

        ssize_t n = read(sockfd, buffer, BUFFER_SIZE); 
        if (n <= 0)
            break;

        buffer[n] = '\0';
        printf("Эхо от сервера: %s", buffer);
    }

    close(sockfd);
    return 0;
}
