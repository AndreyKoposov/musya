#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>


int main() {
    printf("Started...\n");

    const char buffer[256];

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((uint16_t)5000);

    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        perror("Socket Create Error\n");
        return -1;
    }

    if (bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind Error\n");
        return -1;
    }

    if (listen(s, 10) < 0) {
        perror("Listen Error\n");
        return -1;
    }

    printf("Listen...\n");

    while(1) {
        // Accept
        int client_s = accept(s, NULL, NULL);
        if (client_s < 0) {
            perror("Accept Error\n");
            continue;
        }
        printf("Accepted new connection\n");

        // Read
        int read = recv(client_s, (void*)buffer, sizeof(buffer), 0);
        if (read < 0) {
            printf("Recieve Error\n");
            close(client_s);
            continue;
        }
        printf("Read %d byte from %d\n", read, client_s);

        // Send
        int sent = send(client_s, (void*)buffer, read, 0);
        if (sent < 0) {
            printf("Send Error\n");
            close(client_s);
            continue;
        }
        printf("Sent %d byte to %d\n", sent, client_s);

        // Close
        if (close(client_s) < 0) {
            perror("Close Error\n");
        }
        printf("Closed connection\n");
    }
}