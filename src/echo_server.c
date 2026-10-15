#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <arpa/inet.h>

#define BUF_SIZE 20
#define BACKLOG  10

int client_s;
bool connected = false;

void sigintHandler(int signal) {
    printf("Cought signal %d\n", signal);
    printf("Stoped...\n");

    if (connected) {
        if (close(client_s) < 0) {
            perror("Close Error\n");
            exit(1);
        }
        printf("Closed connection\n");
    }

    exit(0);
}

void close_client() {
    if (close(client_s) < 0) {
        perror("Close Error");
    }
    connected = false;
}

int main() {
    int client_counter = 0;

    if (signal(SIGINT, sigintHandler) == SIG_ERR) {
        perror("SIGINT Handler Error");
        return -1;
    }

    printf("Started...\n");

    struct sockaddr_in server_addr;
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

    if (bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind Error");
        exit(EXIT_FAILURE);
    }

    if (listen(s, BACKLOG) < 0) {
        perror("Listen Error");
        exit(EXIT_FAILURE);
    }

    printf("Listen...\n");

    while(1) {
        // Accept
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_s = accept(s, (struct sockaddr*)&client_addr, &addr_len);
        if (client_s < 0) {
            perror("Accept Error");
            continue;
        }
        connected = true;
        client_counter++;

        // Print client info
        printf("\n====\nAccepted new connection №%d\nIP: %s\nPORT: %d\n===\n\n", 
            client_counter, 
            inet_ntoa(client_addr.sin_addr), 
            client_addr.sin_port);

        // Read/Send
        char buffer[BUF_SIZE];
        ssize_t bytes = 0;
        while ((bytes = recv(client_s, buffer, sizeof(buffer), 0)) > 0) {
            if (bytes < 0) {
                perror("Recieve Error");
                close_client();
                continue;
            }
            printf("Read %ld byte from %d: %s\n", bytes, client_s, buffer);
            
            // Send
            ssize_t sent = 0;
            while (sent < bytes) {
                ssize_t k = send(client_s, buffer, sizeof(buffer), 0);
                if (sent < 0) {
                    printf("Send Error");
                    close_client();
                    continue;
                }
                sent += k;
                printf("Sent %ld byte to %d: %s\n", bytes, client_s, buffer);
            }
        }

        // Close
        close_client();
        printf("Closed connection\n");
    }
}