#include "sysinfo.h"

BOOL IsWow64() {
    BOOL bIsWow64 = FALSE;

    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")),
                                                                                "IsWow64Process");

    if (NULL != fnIsWow64Process) {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
            //handle error
        }
    }
    return bIsWow64;
}

int get_arch(struct buffer *out) {
    if (IsWow64()) {
        char *arch_64 = "x86_64";
        size_t usize = strlen(arch_64);
        out->ptr = malloc(usize);
        memcpy(out->ptr, arch_64, usize);

        out->size = usize;
    } else {
        char *arch_32 = "i386";
        size_t usize = strlen(arch_32);
        out->ptr = malloc(usize);
        memcpy(out->ptr, arch_32, usize);

        out->size = usize;
    }

    return 1;
}

int get_platform(struct buffer *out) {
    char *platform = "Windows";
    size_t usize = strlen(platform);
    out->ptr = malloc(usize);
    memcpy(out->ptr, platform, usize);
    out->size = 7;

    return 1;
}

int get_mac(struct buffer *out) {
    IP_ADAPTER_INFO *info = NULL, *pos;
    DWORD size = 0;

    GetAdaptersInfo(info, &size);

    char *format_st = "%02X:%02X:%02X:%02X:%02X:%02X";

    info = (IP_ADAPTER_INFO *) malloc(size);

    out->ptr = malloc(18);
    out->ptr[17] = 0;
    out->size = 17;

    int ret = GetAdaptersInfo(info, &size);

    if (ret) {
        snprintf(out->ptr, 18, format_st, 0, 0, 0, 0, 0, 0);
        return 0;
    }

    for (pos = info; pos != NULL; pos = pos->Next) {
        snprintf(out->ptr, 18, format_st, pos->Address[0], pos->Address[1], pos->Address[2],
                 pos->Address[3], pos->Address[4], pos->Address[5]);
    }

    free(info);
    return 1;
}
