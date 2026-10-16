#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <arpa/inet.h>
#include <string.h>

#include "http.h"


#define BUF_SIZE 4096
#define BACKLOG  10

int client_s = -1;
int s = -1;

void close_client() {
    if (close(client_s) < 0) {
        perror("Close Error");
    }
    client_s = -1;
    printf("Closed connection\n");
}

void sigintHandler(int signal) {
    printf("Cought signal %d\n", signal);
    printf("Stoped...\n");

    if (client_s >= 0)
        close_client();

    if (close(s) < 0)
        exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}

void send_all(int fd, char* buffer) {
    unsigned long sent = 0;
    size_t res_len = strlen(buffer);

    while (sent < res_len) {
        ssize_t k = send(fd, buffer+sent, res_len-sent, 0);
        if (k < 0) {
            perror("Send Error");
            break;
        }
        printf("<<< Sent %zd byte to %d\n", k, fd);
        sent += k;
    }
}

int main() {
    int client_counter = 0;

    //Setup sigint handler
    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        perror("SIGINT Handler Error");
        exit(EXIT_FAILURE);
    }

    printf("Started...\n");

    // Socket creation
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((uint16_t)5000);

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        perror("Socket Create Error");
        exit(EXIT_FAILURE);
    }

    // Reuse port
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind socket
    if (bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind Error");
        exit(EXIT_FAILURE);
    }

    // Listen on socket
    if (listen(s, BACKLOG) < 0) {
        perror("Listen Error");
        exit(EXIT_FAILURE);
    }

    printf("Listen...\n");

    // Main cycle
    while(1) {
        // Accept
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        client_s = accept(s, (struct sockaddr*)&client_addr, &addr_len);
        if (client_s < 0) {
            perror("Accept Error");
            continue;
        }
        client_counter++;

        // Print client info
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        printf("\n====\nAccepted new connection №%d\nIP: %s\nPORT: %d\n===\n\n", 
            client_counter, 
            ip, 
            ntohs(client_addr.sin_port));

        // Read
        char buffer[BUF_SIZE] = {0};
        ssize_t bytes = 0;

        bytes = recv(client_s, buffer, sizeof(buffer)-1, 0);
        if (bytes < 0) {
            perror("Recieve Error");
            close_client();
            continue;
        }
        if (bytes == 0) {
            printf("Connection closed by client\n");
            close_client();
            continue;
        }
        buffer[bytes] = '\0';
        printf(">>> Read %zd byte from %d\n", bytes, client_s);
        
        // Send
        char body[] = "<h1>Hello, World!</h1><h2>This is a body</h2>";
        size_t cap = sizeof(buffer);
        memset(buffer, 0, cap);

        add_status(buffer, cap, 200);
        add_content_type(buffer, cap);
        add_date(buffer, cap);
        add_content_length(buffer, cap, strlen(body));
        add_connection_close(buffer, cap);
        add_body(buffer, cap, body);

        send_all(client_s, buffer);

        // Close
        close_client();
    }
}