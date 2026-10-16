void add_status(char* response, size_t cap, int status);

void add_content_type(char* response, size_t cap);

void add_content_length(char* response, size_t cap, int length);

void add_connection_close(char* response, size_t cap);

void add_body(char* response, size_t cap, char* body);

void add_date(char* response, size_t cap);