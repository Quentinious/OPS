// udp_echo_both.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

void run_server() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP сервер запущен на порту %d\n", PORT);

    while (1) {
        socklen_t len = sizeof(client_addr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr *)&client_addr, &len);
        if (n < 0) {
            perror("recvfrom error");
            continue;
        }

        buffer[n] = '\0';
        printf("Получено от [%s:%d]: %s",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client_addr, len);
    }

    close(sockfd);
}

void run_client() {
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
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (1) {
        printf("Введите сообщение: ");
        if (!fgets(buffer, BUFFER_SIZE, stdin)) break;

        sendto(sockfd, buffer, strlen(buffer), 0,
               (const struct sockaddr *)&server_addr, sizeof(server_addr));

        socklen_t len = sizeof(server_addr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr *)&server_addr, &len);

        buffer[n] = '\0';
        printf("Ответ: %s", buffer);
    }

    close(sockfd);
}

int main() {
    int choice;
    printf("Выберите режим:\n");
    printf("1 — Сервер\n");
    printf("2 — Клиент\n");
    printf("Ваш выбор: ");
    scanf("%d", &choice);
    getchar(); // съедаем перевод строки после scanf

    if (choice == 1) {
        run_server();
    } else if (choice == 2) {
        run_client();
    } else {
        printf("Неверный выбор\n");
    }

    return 0;
}
