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


#define BUF_SIZE 4096
#define BACKLOG  10

int client_s;

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

    exit(EXIT_SUCCESS);
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

    int s = socket(AF_INET, SOCK_STREAM, 0);

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

        // Read/Send
        char buffer[BUF_SIZE] = {0};
        ssize_t bytes = 0;
        while (1) {
            bytes = recv(client_s, buffer, sizeof(buffer)-1, 0);
            if (bytes < 0) {
                perror("Recieve Error");
                break;
            }
            if (bytes == 0) {
                printf("Connection closed by client\n");
                break;
            }
            buffer[bytes] = '\0';
            printf(">>> Read %ld byte from %d: %s\n", bytes, client_s, buffer);
            
            // Send
            ssize_t sent = 0;
            while (sent < bytes) {
                ssize_t k = send(client_s, buffer+sent, bytes-sent, 0);
                if (k < 0) {
                    perror("Send Error");
                    break;
                }
                printf("<<< Sent %ld byte to %d: %s\n", k, client_s, buffer+sent);
                sent += k;
            }
        }

        // Close
        close_client();
    }
}