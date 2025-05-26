// tcp_echo_client.c

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

    // Создание TCP-сокета
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // Подключение к серверу

    // connect() пытается установить TCP-соединение с сервером по адресу server_addr.
    // Параметры:
    // sockfd — дескриптор клиента.
    // (struct sockaddr *)&server_addr — указатель на адрес сервера.
    // sizeof(server_addr) — размер структуры адреса.
    // Если соединение не удаётся, программа завершается.

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

        //         write() отправляет введённые данные серверу через сокет.

        // Параметры:

        // sockfd — дескриптор сокета.

        // buffer — указатель на данные.

        // strlen(buffer) — длина строки.

        // Блокирует процесс до полной отправки данных.

        write(sockfd, buffer, strlen(buffer)); // отправка

        ssize_t n = read(sockfd, buffer, BUFFER_SIZE); // получение
        if (n <= 0)
            break;

        buffer[n] = '\0';
        printf("Эхо от сервера: %s", buffer);
    }

    close(sockfd);
    return 0;
}


// Итог
// Клиент создаёт TCP-сокет.

// Подключается к серверу по IP и порту.

// В цикле отправляет введённые сообщения.

// Получает от сервера их же (эхо).

// Работает, пока не будет EOF на вводе или соединение не закроется.

