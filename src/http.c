#include "http.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void add_status(char* response, int status) {
    char status_str[4];
    //itoa(status, status_str, 10);
    snprintf(status_str, sizeof(status_str), "%d", status);

    strcat(response, "HTTP/1.1 ");
    strcat(response, status_str);
    if (status == 200)
        strcat(response, " OK\r\n");
    else
        strcat(response, " Not found\r\n");
}

void add_content_type(char* response) {

}

void add_content_length(char* response, int length) {

}

void add_connection_close(char* response) {

}

void add_body(char* response, const char* body) {

}