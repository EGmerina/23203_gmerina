#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define PORT 12345


int main()
{

    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    ssize_t n;

    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT); // преобразуем в сетевой порядок (big endian)

    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Ошибка при привязке сокета");
        close(sockfd);
        exit(1);
    }

    printf("UDP эхо-сервер запущен на порту %d\n", PORT);

    while (1)
    {
        client_len = sizeof(client_addr);

        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                     (struct sockaddr *)&client_addr, &client_len);
        if (n < 0)
        {
            perror("Ошибка при получении данных");
            continue;
        }

        buffer[n] = '\0';
        printf("Получено сообщение от %s:%d: %s\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               buffer);

        if (sendto(sockfd, buffer, n + 1, 0,
                   (const struct sockaddr *)&client_addr, client_len) < 0)
        {
            perror("Ошибка при отправке данных");
        }
        else
        {
            printf("Отправлено эхо-сообщение клиенту\n");
        }
    }

    return 0;
}