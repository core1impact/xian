#include "sysinfo.h"

void get_system_info(struct system_info *sysinfo) { // FIXME
    struct buffer arch = {0};
    struct buffer platform = {0};
    struct buffer mac = {0};

    get_arch(&arch);
    get_platform(&platform);
    get_mac(&mac);

    buf_to_str(&arch);
    buf_to_str(&platform);
    buf_to_str(&mac);

    sysinfo->arch = malloc(arch.size);
    sysinfo->platform = malloc(platform.size);
    sysinfo->mac = malloc(mac.size );

    memcpy(sysinfo->arch, arch.ptr, arch.size);
    memcpy(sysinfo->platform, platform.ptr, platform.size);
    memcpy(sysinfo->mac, mac.ptr, mac.size);

    struct buffer sysinfo_buf = {0};

    get_sysinfo_str(&sysinfo_buf.ptr);

    sysinfo_buf.size = strlen(sysinfo_buf.ptr);

    char *id = 0;

    md5(&sysinfo_buf, &id);

    sysinfo->id = malloc(33);
    memcpy(sysinfo->id, id, 33);

    free(arch.ptr);
    free(platform.ptr);
    free(mac.ptr);
    free(sysinfo_buf.ptr);
    free(id);
}

void get_sysinfo_str(char **sysinfo) {
    size_t size = strlen(VERSION) + strlen(g_sysinfo.arch) + strlen(g_sysinfo.platform) + strlen(g_sysinfo.mac) + 4;

    *sysinfo = malloc(size);

    snprintf(*sysinfo, size, "%s\n%s\n%s\n%s", g_sysinfo.mac, g_sysinfo.platform, g_sysinfo.arch, VERSION);
}
