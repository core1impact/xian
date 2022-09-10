#ifndef SYSINFO_NIX_H
#define SYSINFO_NIX_H

#if defined(__unix__) || defined(__APPLE__)

#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/sysctl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>

#endif
#if defined(__APPLE__)

#include <net/if_dl.h>

#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../common/buffer.h"

#define MAC_SIZE 18

extern struct system_info g_sysinfo;

int get_arch(struct buffer *arch);

int get_platform(struct buffer *platform);

int get_mac(struct buffer *buf);

#endif
