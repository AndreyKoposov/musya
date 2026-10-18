#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.h"
#include "request.h"


#define MAX_NAME_L 64
#define MAX_VALUE_L 512
#define MAX_HEADERS_N 64

int parse_method(char* header, struct request* req) {
    strncpy(req->method, strtok(header, " "), 16);
    strncpy(req->path, strtok(NULL, " "), 2048);
    strncpy(req->version, strtok(NULL, " "), 16);

    // char* token4 = strtok(NULL, " ");
    // if (token4 != NULL)
    //     return 0;

    return 1;
}

int parse_header(char* header, struct request* req) {
    char* pos = strstr(header, ": ");

    *pos = '\0';
    char* name = header;
    char* value = pos+2;

    struct header pair;
    strcpy(pair.name, name);
    strcpy(pair.value, value);

    if (!strcmp(name, "Content-Length"))
        req->body_len = atoi(pair.value);

    req->headers[req->header_count] = pair;
    req->header_count++;

    return 1;
}

struct request parse_request(char* head) {
    struct request req = {0};

    if (head == NULL) {
        req.is_valid = 0;
        return req;
    }
    
    char tokens[MAX_HEADERS_N][2048] = {0};
    size_t h_count = 0;
    char* token = strtok(head, "\r\n");
    while (token != NULL) {
        strcpy(tokens[h_count], token);
        token = strtok(NULL, "\r\n");
        h_count++;
    }
    
    if (parse_method(tokens[0], &req) == 0) {
        req.is_valid = 0;
        return req;
    }
    for (size_t i = 1; i < h_count; i++) {
        parse_header(tokens[i], &req);
    }

    req.is_valid = 1;
    return req;
}
