#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <errno.h> 

#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024
#define PORT 12345

int main() {
    int server_fd, new_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    int max_sd, activity, i, valread, sd;
    
    for (i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Ошибка привязки сокета");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 5) < 0) {
        perror("Ошибка ожидания соединений");
        exit(EXIT_FAILURE);
    }
    
    printf("Сервер запущен и слушает порт %d...\n", PORT);
    
    while(1) {
       
        FD_ZERO(&readfds);
        
        // Добавляем серверный сокет в набор
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;
        
        // Добавляем клиентские сокеты в набор
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            
            if (sd > max_sd) {
                max_sd = sd;
            }
        }
        
        // Ожидаем активности на одном из сокетов
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        
        if ((activity < 0) && (errno != EINTR)) {
            perror("Ошибка в select");
            continue;
        }
        
        // Если активность на серверном сокете - новое соединение
        if (FD_ISSET(server_fd, &readfds)) {
            if ((new_socket = accept(server_fd, 
                                    (struct sockaddr *)&address, 
                                    (socklen_t*)&addrlen)) < 0) {
                perror("Ошибка принятия соединения");
                continue;
            }
            
            printf("Новое подключение, сокет fd: %d, IP: %s, порт: %d\n",
                  new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
            
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("Добавлен в список сокетов под номером %d\n", i);
                    break;
                }
            }
            
            if (i == MAX_CLIENTS) {
                printf("Достигнуто максимальное количество клиентов\n");
                close(new_socket);
            }
        }
        
    
        for (i = 0; i < MAX_CLIENTS; i++) {
            sd = client_sockets[i];
            
            if (FD_ISSET(sd, &readfds)) {
                // Чтение данных от клиента
                if ((valread = read(sd, buffer, BUFFER_SIZE)) == 0) {
                    // Соединение закрыто
                    getpeername(sd, (struct sockaddr*)&address, 
                               (socklen_t*)&addrlen);
                    printf("Клиент отключился, IP %s, порт %d\n",
                          inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    
                    close(sd);
                    client_sockets[i] = 0;
                } else {
                    // Эхо-ответ
                    buffer[valread] = '\0';
                    send(sd, buffer, strlen(buffer), 0);
                    printf("Отправлен эхо-ответ клиенту %d: %s", i, buffer);
                }
            }
        }
    }
    
    close(server_fd);
    return 0;
}