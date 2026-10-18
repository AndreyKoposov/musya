#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "request.h"


#define MAX_NAME_L 64
#define MAX_VALUE_L 512
#define MAX_HEADERS_N 64

void parse_method(char* header, struct request* req) {
    strncpy(req->method, strtok(header, " "), 16);
    strncpy(req->path, strtok(NULL, " "), 2048);
    strncpy(req->version, strtok(NULL, " "), 16);
}

void parse_header(char* header, struct request* req, size_t n) {
    char* pos = strstr(header, ": ");

    *pos = '\0';
    char* name = header;
    char* value = pos+2;

    struct header pair;
    strcpy(pair.name, name);
    strcpy(pair.value, value);

    if (!strcmp(name, "Content-Length"))
        req->body_len = atoi(pair.value);

    req->headers[n] = pair;
    req->header_count++;
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
    
    parse_method(tokens[0], &req);
    for (size_t i = 1; i < MAX_HEADERS_N; i++) {
        if (strlen(tokens[i]) > 0)
            parse_header(tokens[i], &req, i-1);
    }

    return req;
}

void print_request(struct request* req) {
    printf("Method: %s\nVersion: %s\nPath: %s\nBody len: %lu\nHeaders count: %d\n", 
        req->method, req->version, req->path, req->body_len, req->header_count);
}
