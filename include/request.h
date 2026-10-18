typedef struct header {
    char name[64];
    char value[512];
} header;

typedef struct request {
    char method[16];
    char path[2048];
    char version[16];
    struct header headers[64];
    int header_count;
    const char *body;
    size_t body_len;
} request;

void print_request(struct request* req);
