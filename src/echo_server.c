#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <arpa/inet.h>

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

int main() {
    const char buffer[256];
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
        return -1;
    }

    if (bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind Error");
        return -1;
    }

    if (listen(s, 10) < 0) {
        perror("Listen Error");
        return -1;
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

        // Client info
        printf("Accepted new connection №%d\nIP: %s\nPORT: %d\n", 
            client_counter, 
            inet_ntoa(client_addr.sin_addr), 
            client_addr.sin_port);

        // Read
        int read = recv(client_s, (void*)buffer, sizeof(buffer), 0);
        if (read < 0) {
            perror("Recieve Error");
            close(client_s);
            continue;
        }
        printf("Read %d byte from %d: %s\n", read, client_s, buffer);

        // Send
        int sent = send(client_s, (void*)buffer, read, 0);
        if (sent < 0) {
            printf("Send Error");
            close(client_s);
            continue;
        }
        printf("Sent %d byte to %d\n", sent, client_s);

        // Close
        if (close(client_s) < 0) {
            perror("Close Error");
        }
        connected = false;
        printf("Closed connection\n");
    }
}