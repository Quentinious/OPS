#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);


    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) 
    {
        perror("inet_pton failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("Enter message: ");
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        {
            printf("Error or EOF on input, exiting\n");
            break;
        }

        
        ssize_t sent_bytes = sendto(sockfd, buffer, strlen(buffer), 0,(struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_bytes < 0) {
            perror("sendto failed");
            continue;
        }

        
        socklen_t addr_len = sizeof(server_addr);
        ssize_t recv_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                      (struct sockaddr *)&server_addr, &addr_len);
        if (recv_bytes < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[recv_bytes] = '\0'; 
        printf("Echo: %s", buffer);
    }

    close(sockfd);
    return 0;
}
