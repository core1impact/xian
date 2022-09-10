#include "base64.h"

int is_base64_char(char c) {
    if (c >= 'A' && c <= 'Z') {
        return 1;
    }
    if (c >= 'a' && c <= 'z') {
        return 1;
    }
    if (c >= '0' && c <= '9') {
        return 1;
    }
    if (c == '+' || c == '/' || c == '=') {
        return 1;
    }

    return 0;
}

int base64_encode(const struct buffer *data, struct buffer *out) {
    out->ptr = malloc(BASE64_ENCODE_OUT_SIZE(data->size));

    if (!out->ptr) {
        return 0;
    }

    size_t size = base64_encode_ext(data->ptr, data->size, out->ptr);

    if (!size) {
        free(out->ptr);
        return 0;
    }

    out->size = size;

    return 1;
}

int base64_decode(const struct buffer *data, struct buffer *out) {
    out->ptr = malloc(BASE64_DECODE_OUT_SIZE(data->size));

    if (!out->ptr) {
        return 0;
    }

    size_t size = base64_decode_ext(data->ptr, data->size, out->ptr);

    if (!size) {
        free(out->ptr);
        return 0;
    }

    out->size = size;

    return 1;
}
