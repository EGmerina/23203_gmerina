#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1" 

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t server_len;
    ssize_t n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Ошибка при преобразовании IP адреса");
        close(sockfd);
        exit(1);
    }

    printf("UDP клиент подключен к серверу %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        printf("Введите сообщение (или 'exit' для выхода): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        if (sendto(sockfd, buffer, strlen(buffer), 0,
                  (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("Ошибка при отправке данных");
            continue;
        }

        printf("Сообщение отправлено серверу\n");

        server_len = sizeof(server_addr);
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                    (struct sockaddr *)&server_addr, &server_len);
        if (n < 0) {
            perror("Ошибка при получении данных");
            continue;
        }

        buffer[n] = '\0';
        printf("Получен ответ от сервера: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}