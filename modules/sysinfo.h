#ifndef SYSINFO_H
#define SYSINFO_H

#include <stdlib.h>
#include "../common/buffer.h"
#include "../config.h"
#include "../common/md5.h"

#if defined(__unix__) || defined(__APPLE__)
#include "nix/sysinfo.h"
#elif __WIN32__
#include "win/sysinfo.h"
#endif

struct system_info {
    char *arch;
    char *platform;
    char *mac;
    char *id;
};

extern struct system_info g_sysinfo;

void get_system_info(struct system_info *sysinfo);

void get_sysinfo_str(char **sysinfo);

#endif
