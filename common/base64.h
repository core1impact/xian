#ifndef BASE64_H
#define BASE64_H

#include "../external/base64/base64.h"
#include "buffer.h"

int is_base64_char(char c);

int base64_encode(const struct buffer *data, struct buffer *out);

int base64_decode(const struct buffer *data, struct buffer *out);

#endif
