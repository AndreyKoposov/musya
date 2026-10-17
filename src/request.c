#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define MAX_NAME_L 64
#define MAX_VALUE_L 512
#define MAX_HEADERS_N 64

struct header {
    char name[MAX_NAME_L];
    char value[MAX_VALUE_L];
};

struct request {
    char version[8];
    char method[16];
    char path[2048];
    struct header headers[MAX_HEADERS_N];
    int header_count;
    const char *body;
    size_t body_len;
};

void parse_method(char* header, struct request* req) {
    strcpy(req->method, strtok(header, " "));
    strcpy(req->path, strtok(NULL, " "));
    strcpy(req->version, strtok(NULL, " "));
}

void parse_header(char* header, struct request* req, size_t n) {
   char* pos = strstr(header, ": ");
   
}

struct request parse_request(char* head) {
    struct request req;

    char tokens[MAX_HEADERS_N][2048] = {0};
    size_t h_count = 0;
    char* token = strtok(head, "\r\n");

    while (token != NULL) {
        strcpy(tokens[h_count], token);
        token = strtok(NULL, "\r\n");
        h_count++;
    }

    // for (size_t i = 0; i < 64; i++) {
    //     if (strlen(tokens[i]) > 0)
    //         printf("Header: %s\n", tokens[i]);
    // }

    parse_method(tokens[0], &req);

    return req;
}
