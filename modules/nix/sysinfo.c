#include <unistd.h>
#include "sysinfo.h"

#if defined(__unix__) || defined(__APPLE__)
struct utsname u;
#endif

int get_arch(struct buffer *arch) {
#if defined(__unix__) || defined(__APPLE__)
    uname(&u);
    size_t usize = strlen(u.machine);
    arch->ptr = malloc(usize);
    memcpy(arch->ptr, u.machine, usize);
    arch->size = usize;
#endif
    return 0;
}

int get_platform(struct buffer *platform) {
#if defined(__unix__) || defined(__APPLE__)
    uname(&u);

    size_t usize = strlen(u.sysname);
    platform->ptr = malloc(usize);
    memcpy(platform->ptr, u.sysname, usize);

    platform->size = usize;
#endif
    return 0;
}

int get_mac(struct buffer *buf) {
    char *format_st = "%02x:%02x:%02x:%02x:%02x:%02x";

#if defined(__unix__) || defined(__APPLE__)
    int mib[6];
    size_t len;
    char *buf2;
    unsigned char *ptr;
    struct if_msghdr *ifm;
    struct sockaddr_dl *sdl;
    struct ifaddrs *pfi, *ifa;
    char *iface /* nocrypt */ = "en0";

#if defined(__APPLE__)
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;

#endif
    getifaddrs(&pfi);
    for (ifa = pfi; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET && (0 == (ifa->ifa_flags & IFF_LOOPBACK))) {
            iface = ifa->ifa_name;
        }
    }

#if defined(__APPLE__)

    freeifaddrs(pfi);

    if ((mib[5] = if_nametoindex(iface)) == 0) {
        return 0;
    }

    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        return 0;
    }

    if ((buf2 = malloc(len)) == NULL) {
        return 0;
    }

    if (sysctl(mib, 6, buf2, &len, NULL, 0) < 0) {
        return 0;
    }

    ifm = (struct if_msghdr *) buf2;
    sdl = (struct sockaddr_dl *) (ifm + 1);
    ptr = (unsigned char *) LLADDR(sdl);
    buf->ptr = malloc(MAC_SIZE);


    snprintf(buf->ptr, MAC_SIZE, format_st, *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3), *(ptr + 4), *(ptr + 5));
#elif defined(__unix__) || defined(__linux__)

    int fd;
    struct ifreq ifr;
    unsigned char *mac = NULL;

    memset(&ifr, 0, sizeof(ifr));

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);

    if (0 == ioctl(fd, SIOCGIFHWADDR, &ifr)) {
        mac = (unsigned char *) ifr.ifr_hwaddr.sa_data;

        buf->ptr = malloc(MAC_SIZE);
        snprintf(buf->ptr, MAC_SIZE, format_st, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }

    close(fd);

#endif

#endif
    buf->size = MAC_SIZE;
    return 0;
}
