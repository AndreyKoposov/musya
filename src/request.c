#include <stdio.h>
#include <string.h>

#include "request.h"


void print_request(struct request* req) {
    printf("Method: %s\nVersion: %s\nPath: %s\nBody len: %lu\nHeaders count: %d\n", 
        req->method, req->version, req->path, req->body_len, req->header_count);
    printf("Headers:\n");
    for (int i = 0; i < req->header_count; i++) {
        printf("\t<%s: %s>\n", req->headers[i].name, req->headers[i].value);
    }
    printf("Body:\n%s\n", req->body);
}

int process_method(char* method) {
    if (strcmp(method, "GET") == 0)
        return 1;
    else if (strcmp(method, "POST") == 0)
        return 1;
    else if (strcmp(method, "HEAD") == 0)
        return 1;
    else if (strcmp(method, "PUT") == 0)
        return 1;
    else if (strcmp(method, "DELETE") == 0)
        return 1;
    else if (strcmp(method, "OPTIONS") == 0)
        return 1;
    else
        return 0;
}

int process_path(char* method) {
    if (strlen(method) > 0) 
        return 1;
    else
        return 0;
}

int process_version(char* method) {
    if (strcmp(method, "HTTP/1.1") == 0)
        return 1;
    else if (strcmp(method, "HTTP/1.0") == 0)
        return 1;
    else
        return 0;
}

// int process_header(struct header line, char* version) {

// }

int process_headers(struct request* req) {
    int has_host = 0;
    for (int i = 0; i < req->header_count; i++) {
        struct header header = req->headers[i];

        if (strcmp(header.name, "Host") == 0)
            has_host = 1;
    }

    if (strcmp(req->version, "HTTP/1.1") == 0 && !has_host)
        return 0;

    return 1;
}

int process_request(struct request* req) {
    if (!process_method(req->method))
        return 501;
    if (!process_path(req->path))
        return 400;
    if (!process_version(req->version))
        return 400;
    if (!process_headers(req))
        return 400;

    return 200;
}