#include "exec.h"

int exec(const char *command, struct buffer *buffer) {
    char *mode = "r";

    FILE *fp = popen(command, mode);

    if (!fp) {
        return 0;
    }

    buffer->ptr = malloc(STDOUT_BLOCK_SIZE);
    size_t malloc_size = STDOUT_BLOCK_SIZE;

    int ret = 1;
    char ch = 0;

    while (!feof(fp)) {
        ch = fgetc(fp);

        if (buffer->size >= malloc_size) {
            size_t new_size = malloc_size + STDOUT_BLOCK_SIZE;

            if (new_size > STDOUT_MAX_BLOCK_SIZE) {
                ret = 0;
                goto error;
            }

            buffer->ptr = realloc(buffer->ptr, new_size);

            if (!buffer->ptr) {
                ret = 0;
                goto error;
            }

            malloc_size += STDOUT_BLOCK_SIZE;
        }

        memcpy(buffer->ptr + buffer->size, &ch, 1);
        buffer->size++;
    }

    if (buffer->size) {
        buffer->size--;
        buffer->ptr = realloc(buffer->ptr, buffer->size);
    }

    goto clean;

    error:

    if (buffer->size) {
        free(buffer->ptr);
        buffer->size = 0;
    }

    clean:

    pclose(fp);
    return ret;
}
