#ifndef MD5_EXT_H
#define MD5_EXT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void md5_ext(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest);

#endif
