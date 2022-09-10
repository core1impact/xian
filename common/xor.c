#include "xor.h"

void xor(char *buf, size_t size) {
    char key[] = {1};
    int key_size = 1;

    for (size_t i = 0; i < size; ++i) {
        buf[i] = buf[i] ^ key[i % key_size];
    }
}
