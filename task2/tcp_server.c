// tcp_echo_server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12346
#define BUFFER_SIZE 1024

// каждого клиента обрабатывает

void handle_client(int client_sock)
{
    char buffer[BUFFER_SIZE];
    ssize_t n;

    printf("Обработка клиента в процессе %d\n", getpid());

//     Назначение: Считывает данные из файла или сокета.

// Параметры:

// fd: дескриптор файла или сокета.

// buf: указатель на буфер, куда будут записаны данные.

// count: максимальное количество байт для чтения.

// Возвращаемое значение: количество реально прочитанных байт, 0 — если конец файла (или клиент закрыл соединение), -1 — ошибка.

    while ((n = read(client_sock, buffer, BUFFER_SIZE)) > 0)
    {
        //Назначение: Записывает данные в файл или сокет.

        // Параметры:

        // fd: дескриптор файла или сокета.

        // buf: указатель на данные для отправки.

        // count: количество байт для записи.

        // Возвращаемое значение: количество реально записанных байт, -1 — ошибка.
        write(client_sock, buffer, n); // эхо-ответ
    }

    printf("Клиент отключился. Закрытие соединения в процессе %d\n", getpid());
    close(client_sock);
    exit(0);
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

    // Избегаем зомби-процессов
    signal(SIGCHLD, SIG_IGN);

    //     Назначение: Устанавливает обработчик сигнала.
    // Параметры:

    // signum: номер сигнала (здесь SIGCHLD — сигнал завершения дочернего процесса).

    // handler: функция-обработчик или специальное значение SIG_IGN (игнорировать).

    // Зачем: чтобы избежать появления зомби-процессов, игнорируем сигнал завершения потомков.






    // Создание TCP-сокета

    //     domain: Семейство протоколов, обычно AF_INET для IPv4.

    // type: Тип сокета. Для TCP — SOCK_STREAM (потоковый), для UDP — SOCK_DGRAM (датаграммный).

    // protocol: Обычно 0 — выбирается автоматически соответствующий протокол (TCP для SOCK_STREAM).

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Подготовка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //     sockfd: дескриптор сокета, созданного через socket().

    // addr: указатель на структуру адреса (struct sockaddr), описывающую IP и порт.

    // addrlen: длина структуры addr.

    // Возвращаемое значение: 0 при успехе, -1 при ошибке.

    // Примечание: На сервере bind() обязателен, чтобы указать, на каком порту и интерфейсе слушать входящие соединения. На клиенте в TCP обычно не нужен (операционная система сама выберет порт).

    // Привязка и запуск на прослушивание
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    //     sockfd: дескриптор ранее созданного и связанного сокета.
    // backlog: максимальное количество ожидающих подключений, которые система может держать в очереди.

    if (listen(server_sock, 5) < 0)
    {
        perror("listen failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("TCP эхо-сервер запущен на порту %d\n", PORT);

    while (1)
    {
        //Назначение: Принимает входящее соединение.
        // Параметры:
        // sockfd: дескриптор слушающего сокета.
        // addr: указатель на структуру, куда будет записан адрес клиента.
        // addrlen: указатель на размер структуры addr; при вызове содержит размер, при выходе обновляется реальный размер.
        // Возвращаемое значение: новый дескриптор сокета для общения с клиентом, или -1 при ошибке.
        // Особенности: Эта функция блокируется, пока не придёт новое соединение.

        socklen_t len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &len);

        if (client_sock < 0)
        {
            perror("accept failed");
            continue;
        }

        //Назначение: Преобразует IPv4 адрес из бинарного вида в строку формата "x.x.x.x".
        // Используется: для вывода IP клиента в человекочитаемом виде.
        printf("Новое соединение от %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Создание дочернего процесса для обслуживания клиента
        pid_t pid = fork();
        if (pid == 0)
        {
            close(server_sock); // Дочернему не нужен серверный сокет
            handle_client(client_sock);
        }
        else if (pid > 0)
        {
            close(client_sock); // Родителю не нужен клиентский сокет
        }
        else
        {
            perror("fork failed");
        }
    }

    close(server_sock);
    return 0;
}


// Важные замечания:
// Блокирующие вызовы:

// accept() — блокируется до нового соединения.

// read() — блокируется, пока не появятся данные или не закроют сокет.

// Поток управления:

// В родителе — цикл ожидания и принятия клиентов.

// Для каждого клиента — новый дочерний процесс с независимой обработкой.

// Порядок байтов:

// Все сетевые поля (IP, порт) должны быть в сетевом порядке (big-endian).

// Внутри машины порядок может отличаться (little-endian или big-endian).


