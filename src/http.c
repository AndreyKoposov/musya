#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "http.h"


void get_gmt_date(char* date, size_t max_size) {
    time_t current_t;
    struct tm* ftime;

    time(&current_t);
    ftime = gmtime(&current_t);

    strftime(date, max_size, "%a, %d %b %Y %H:%M:%S GMT", ftime);
}

void add_status(char* response, int status) {
    char status_str[4];
    snprintf(status_str, sizeof(status_str), "%d", status);

    strcat(response, "HTTP/1.1 ");
    strcat(response, status_str);
    if (status == 200)
        strcat(response, " OK\r\n");
    else
        strcat(response, " Not found\r\n");
}

void add_content_type(char* response) {
    strcat(response, "Content-Type: text/html; charset=utf-8\r\n");
}

void add_content_length(char* response, int length) {
    char length_str[10];
    snprintf(length_str, sizeof(length_str), "%d", length);

    strcat(response, "Content-Length: ");
    strcat(response, length_str);
    strcat(response, "\r\n");
}

void add_connection_close(char* response) {
    strcat(response, "Connection: close\r\n");
}

void add_body(char* response, char* body) {
    strcat(response, "\r\n");
    strcat(response, body);
}

void add_date(char* response) {
    strcat(response, "Date: ");

    char date[80];
    get_gmt_date(date, 80);
    strcat(response, date);

    strcat(response, "\r\n");
}