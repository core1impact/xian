#include "md5.h"

int md5(const struct buffer *buf, char **out) {
    *out = malloc(MD5_STR_SIZE);

    if (!*out) {
        return 0;
    }

    unsigned char data[MD5_BUF_SIZE];
    memset(data, 0, MD5_BUF_SIZE);

    md5_ext(buf->ptr, buf->size, data);

    int ret = 0;

    for (int i = 0; i < MD5_BUF_SIZE; ++i) {
        if (data[i]) {
            ret = 1;
            break;
        }
    }

    if (!ret) {
        free(*out);
        return 0;
    }

    snprintf(*out, MD5_STR_SIZE, "%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x",
             data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
             data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]
    );

    return 1;
}
