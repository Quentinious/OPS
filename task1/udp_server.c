#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() 
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);        
    server_addr.sin_addr.s_addr = INADDR_ANY;


    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("UDP echo server started on port %d\n", PORT);

    while (1)
    {
        socklen_t len = sizeof(client_addr);


        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr *)&client_addr, &len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }



        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client_addr, len);
    }

    close(sockfd);
    return 0;
}
