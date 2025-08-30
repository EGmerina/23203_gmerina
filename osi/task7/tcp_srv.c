#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 12345

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    while ((bytes_read = read(client_sock, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("[PID %d] Получено: %s", getpid(), buffer);
        
        write(client_sock, buffer, bytes_read);
        printf("[PID %d] Отправлено эхо: %s", getpid(), buffer);
    }
    
    if (bytes_read == 0) {
        printf("[PID %d] Клиент отключился\n", getpid());
    } else {
        perror("Ошибка чтения");
    }
    
    close(client_sock);
    exit(0);
}

int main() {
    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) < 0) {
        perror("Ошибка при listen");
        close(sockfd);
        exit(1);
    }

    printf("TCP эхо-сервер запущен на порту %d\n", PORT);

    while (1) {
        client_len = sizeof(client_addr);
        
        client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("Ошибка при accept");
            continue;
        }
        
        printf("Новое соединение от %s:%d\n", 
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port));

    
        pid = fork();
        if (pid < 0) {
            perror("Ошибка при fork");
            close(client_sock);
            continue;
        }
        
        if (pid == 0) { // Дочерний процесс
            close(sockfd); // Закрываем слушающий сокет в дочернем процессе
            handle_client(client_sock);
        } else { 
            close(client_sock); // Закрываем клиентский сокет в родительском процессе
        }
    }

    close(sockfd);
    return 0;
}