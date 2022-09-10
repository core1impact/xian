#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

struct buffer {
    char *ptr;
    size_t size;
};

void buf_to_str(struct buffer *buf);

void str_to_buf(struct buffer *str);

#endif
