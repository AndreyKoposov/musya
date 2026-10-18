#include <stdio.h>
#include <string.h>
#include <strings.h>

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
        return 501;
}

int process_path(char* method) {
    if (strlen(method) > 0) 
        return 1;
    else
        return 400;
}

int process_version(char* method) {
    if (strcmp(method, "HTTP/1.1") == 0)
        return 1;
    else if (strcmp(method, "HTTP/1.0") == 0)
        return 1;
    else
        return 400;
}

int process_header(struct header* header) {
    if (strcasecmp(header->name, "Content-Type") == 0)
        return 1;
    else if (strcasecmp(header->name, "Date") == 0)
        return 1;
    else if (strcasecmp(header->name, "Content-Length") == 0)
        return 1;
    else if (strcasecmp(header->name, "Connection") == 0)
        return 1;
    else if (strcasecmp(header->name, "User-Agent") == 0)
        return 1;
    else if (strcasecmp(header->name, "Host") == 0)
        return 1;
    else if (strcasecmp(header->name, "Accept") == 0)
        return 1;
    else
        return 400;
}

int process_headers(struct request* req) {
    int has_host = 0;
    int status = 0;
    for (int i = 0; i < req->header_count; i++) {
        struct header header = req->headers[i];

        if ((status = process_header(&header)) != 1)
            return status;

        if (strcmp(header.name, "Host") == 0)
            has_host = 1;
    }

    if (strcmp(req->version, "HTTP/1.1") == 0 && !has_host)
        return 400;

    return 1;
}

int process_request(struct request* req) {
    int status = 0;
    if ((status = process_method(req->method)) != 1)
        return status;
    if ((status = process_path(req->path)) != 1)
        return status;
    if ((status = process_version(req->version)) != 1)
        return status;
    if ((status = process_headers(req)) != 1)
        return status;

    return 200;
}