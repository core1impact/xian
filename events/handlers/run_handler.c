#include "run_handler.h"

int run_handler(struct buffer *command) {
    struct buffer buf = {0};

    int ret = exec(command->ptr, &buf);

    if (!ret) {
        return 0;
    }

    if (buf.size) {
        char *run_cmd = "run";
        ret = transport_send(run_cmd, &buf, 0, 0);
        free(buf.ptr);
    }

    return ret;
}
