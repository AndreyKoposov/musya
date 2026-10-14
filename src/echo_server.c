#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char argv[]) {
    const char buffer[256];

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((uint16_t)80);

    int s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        errno("\nSocket Create Error");
        return -1;
    }

    if (bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        errno("\nBind Error");
        return -1;
    }

    if (listen(s, 10) < 0) {
        errno("Listen Error");
        return -1;
    }

    while(1) {
        // Accept
        int client_s = accept(s, NULL, NULL);
        if (client_s < 0) {
            errno("Accept Error");
            continue;
        }

        // Read
        int read = recv(client_s, (void*)buffer, 256, NULL);
        if (read < 0) {
            printf("Recieve Error");
            close(client_s);
            continue;
        }

        // Send
        int sent = send(client_s, (void*)buffer, read, NULL);
        if (sent < 0) {
            printf("Send Error");
            close(client_s);
            continue;
        }

        // Close
        if (close(client_s) < 0) {
            errno("Close Error");
        }
    }
}