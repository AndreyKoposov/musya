#include <stdlib.h>


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