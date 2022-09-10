#ifndef EXEC_HANDLER_NIX_H
#define EXEC_HANDLER_NIX_H

#if defined(__linux__)

#include <sys/prctl.h>
#include <syscall.h>

#endif

#include <stdio.h>


#include "../../../config.h"
#include "../../../common/buffer.h"

G_STR(nix_process_name);

int exec_handler(struct buffer *buf);

int run_memory(struct buffer *buf);

#endif
