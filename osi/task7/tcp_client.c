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
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при connect");
        close(sockfd);
        exit(1);
    }

    printf("Подключено к серверу %s:%d\n", SERVER_IP, SERVER_PORT);

    while (1) {
        printf("Введите сообщение (или 'exit' для выхода): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }

        if (write(sockfd, buffer, strlen(buffer)) < 0) {
            perror("Ошибка при отправке данных");
            break;
        }

        bytes_read = read(sockfd, buffer, BUFFER_SIZE - 1);
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                printf("Сервер закрыл соединение\n");
            } else {
                perror("Ошибка при чтении данных");
            }
            break;
        }

        buffer[bytes_read] = '\0';
        printf("Ответ сервера: %s", buffer);
    }

    close(sockfd);
    return 0;
}