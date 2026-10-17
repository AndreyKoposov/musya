struct request;

void extract_headers(char* block, size_t n, char* headers[64][2048]);

void parse_request(char* head);