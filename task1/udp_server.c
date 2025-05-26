#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() 
{

//     sockfd — файловый дескриптор сокета, который будет использоваться для связи.
// server_addr — структура, описывающая адрес сервера (IP + порт).
// client_addr — структура, в которую мы будем записывать адрес клиента, от которого пришли данные.
// buffer — массив байт для хранения принятых данных.

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];


//     AF_INET — семейство протоколов IPv4.

// SOCK_DGRAM — тип сокета — датаграммы (UDP).

// 0 — протокол (0 означает выбрать протокол автоматически для UDP).
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }


// Заполняем структуру server_addr для привязки сокета:

// sin_family = AF_INET — IPv4.

// sin_port = htons(PORT) — порт в сетевом порядке байтов (Big Endian). htons — переводит из порядка хоста (возможно Little Endian) в сетевой.

// sin_addr.s_addr = INADDR_ANY — слушать на всех доступных IP-адресах (0.0.0.0).

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);         //	Преобразует 16-битное число из порядка хоста в сетевой
    server_addr.sin_addr.s_addr = INADDR_ANY;



//     bind() — связывает сокет с конкретным IP и портом (то, что мы задали в server_addr).

// Без bind() сервер не будет знать, на каком порту слушать.

// Если ошибка — выводим и завершаем.

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("UDP echo server started on port %d\n", PORT);

    while (1)
    {
        socklen_t len = sizeof(client_addr);

        //         recvfrom() — получает UDP-пакет из сокета:

        // sockfd — сокет.

        // buffer — куда писать принятые данные.

        // BUFFER_SIZE — максимальное число байт.

        // 0 — флаги (нет особых).

        // (struct sockaddr *)&client_addr — сюда запишется адрес клиента, от которого пришли данные.

        // &len — размер структуры клиента (и обновится, если адрес клиента короче/длиннее).

        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                             (struct sockaddr *)&client_addr, &len);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }

//         sendto() — отправляем данные обратно клиенту:

// sockfd — сокет.

// buffer — данные, которые получили.

// n — количество байт, которое нужно отправить (то, что приняли).

// 0 — флаги.

// (struct sockaddr *)&client_addr — адрес получателя.

// len — длина адреса.

        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&client_addr, len);
    }

    close(sockfd);
    return 0;
}
