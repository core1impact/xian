#ifndef SYSINFO_WIN_H
#define SYSINFO_WIN_H

#include <iphlpapi.h>
#include <stdio.h>
#include <windows.h>

#include "../../common/buffer.h"

typedef BOOL (WINAPI
*LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

int get_arch(struct buffer *out);

int get_platform(struct buffer *out);

int get_mac(struct buffer *out);

#endif
