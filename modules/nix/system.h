#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "../../config.h"

G_STR(nix_bin_path);

#if defined(__linux__) || defined(__APPLE__)

#include <sys/utsname.h>

#endif

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <copyfile.h>
#else

#include <sys/sendfile.h>

#endif

#if defined(__linux__)

#include <sys/prctl.h>
#include <syscall.h>

#endif

int persistent_launch_agent(char *agent);

int into_background();

int set_process_name(char *name);

