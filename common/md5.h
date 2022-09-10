#ifndef MD5_H
#define MD5_H

#include <stdio.h>

#include "../common/buffer.h"
#include "../external/md5/md5.h"

#define MD5_BUF_SIZE 16
#define MD5_STR_SIZE 33

int md5(const struct buffer *buf, char **out);

#endif
