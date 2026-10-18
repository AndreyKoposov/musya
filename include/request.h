typedef struct header {
    char name[64];
    char value[512];
} header;

typedef struct request {
    char method[16];
    char path[2048];
    char version[8];
    struct header headers[64];
    int header_count;
    const char *body;
    size_t body_len;
} request;

void extract_headers(char* block, size_t n, char* headers[64][2048]);

struct request parse_request(char* head);

void print_request(struct request* req);