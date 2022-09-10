#ifndef EXEC_HANDLER_WIN_H
#define EXEC_HANDLER_WIN_H

#include <windows.h>

#include "../../../config.h"
#include "../../../common/buffer.h"

extern char win_bin_path[];

int exec_handler(struct buffer *buf);

#endif
