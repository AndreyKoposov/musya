#include <stdio.h>

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