#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "http.h"


#define TMP_SIZE 64

void get_gmt_date(char* date, size_t max_size) {
    time_t current_t;
    struct tm* ftime;

    time(&current_t);
    ftime = gmtime(&current_t);

    strftime(date, max_size, "%a, %d %b %Y %H:%M:%S GMT", ftime);
}

void add_status(char* response, size_t cap, int status) {
    char tmp[TMP_SIZE];
    snprintf(tmp, sizeof(tmp), "HTTP/1.1 %d %s\r\n", status, status == 200 ? "OK" : "Not Found");
    strncat(response, tmp, cap - strlen(response) - 1);
}

void add_content_type(char* response, size_t cap) {
    strncat(response, "Content-Type: text/html; charset=utf-8\r\n", cap - strlen(response) - 1);
}

void add_content_length(char* response, size_t cap, int length) {
    char tmp[TMP_SIZE];
    snprintf(tmp, sizeof(tmp), "Content-Length: %d\r\n", length);
    strncat(response, tmp, cap - strlen(response) - 1);
}

void add_connection_close(char* response, size_t cap) {
    strncat(response, "Connection: close\r\n", cap - strlen(response) - 1);
}

void add_body(char* response, size_t cap, char* body) {
    strncat(response, "\r\n", cap - strlen(response) - 1);
    strncat(response, body, cap - strlen(response) - 1);
}

void add_date(char* response, size_t cap) {
    char tmp[2*TMP_SIZE];
    char date[TMP_SIZE];

    get_gmt_date(date, sizeof(date));
    snprintf(tmp, sizeof(tmp), "Date: %s\r\n", date);
    strncat(response, tmp, cap - strlen(response) - 1);
}