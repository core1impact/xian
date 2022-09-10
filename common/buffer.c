#include "buffer.h"

void buf_to_str(struct buffer *buf) {
    size_t new_len = buf->size + 1;

    buf->ptr = realloc(buf->ptr, new_len);
    buf->ptr[new_len - 1] = 0;

    buf->size = new_len;
}

void str_to_buf(struct buffer *str) {
    size_t new_len = str->size - 1;

    str->ptr = realloc(str->ptr, new_len);
    str->size = new_len;
}
