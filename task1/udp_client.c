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

    // Создаём UDP-сокет
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Заполняем адрес сервера
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

        // Отправляем сообщение серверу
        ssize_t sent_bytes = sendto(sockfd, buffer, strlen(buffer), 0,(struct sockaddr *)&server_addr, sizeof(server_addr));
        if (sent_bytes < 0) {
            perror("sendto failed");
            continue;
        }

        // Получаем ответ от сервера
        socklen_t addr_len = sizeof(server_addr);
        ssize_t recv_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                      (struct sockaddr *)&server_addr, &addr_len);
        if (recv_bytes < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[recv_bytes] = '\0'; // Нуль-терминатор для корректного вывода строки
        printf("Echo: %s", buffer);
    }

    close(sockfd);
    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>


// //<arpa/inet.h> — для работы с сетевыми адресами и функциями inet_pton и т.п.

// #define SERVER_IP "127.0.0.1"
// #define SERVER_PORT 12345
// #define BUFFER_SIZE 1024

// int main() {
//     int sockfd;
//     struct sockaddr_in server_addr;
//     char buffer[BUFFER_SIZE];

//     sockfd = socket(AF_INET, SOCK_DGRAM, 0);

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(SERVER_PORT);
//     inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr); //inet_pton — преобразует строку IP-адреса в двоичный формат и записывает в sin_addr.

//     while (1) 
//     {
//         printf("Enter message: ");

//         //fgets читает строку из стандартного ввода (клавиатуры) в buffer

//         fgets(buffer, BUFFER_SIZE, stdin);




//     //         Отправляем UDP-пакет на сервер:

//     // sockfd — дескриптор нашего UDP-сокета.

//     // buffer — данные для отправки.

//     // strlen(buffer) — длина данных (количество байт).

//     // 0 — флаги (обычно 0).

//     // (struct sockaddr *)&server_addr — адрес, куда отправляем (сервер).

//     // sizeof(server_addr) — размер структуры адреса.

//     // В UDP нет соединения, поэтому передаем адрес с каждым пакетом.
//         sendto(sockfd, buffer, strlen(buffer), 0,(struct sockaddr *)&server_addr, sizeof(server_addr));

//         socklen_t len = sizeof(server_addr);


//         //         recvfrom блокируется, пока не придёт пакет.

//         // Полученные данные записываются в buffer.

//         // len — размер структуры server_addr (будет обновлен фактическим размером адреса источника).

//         // Адрес источника (сервер) записывается в server_addr (обычно совпадает с тем, кому мы отправили).

//         // n — количество принятых байт.
//         ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,(struct sockaddr *)&server_addr, &len);
//         buffer[n] = '\0';
//         printf("Echo: %s", buffer);
//     }

//     close(sockfd);
//     return 0;
// }
