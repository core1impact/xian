#ifndef UPDATE_HANDLER_WIN_H
#define UPDATE_HANDLER_WIN_H

#include "../../../config.h"
#include "../../../common/buffer.h"
#include "../../../common/helpers/win/file.h"
#include "../../../common/helpers/win/process.h"

extern char win_bin_path[];

extern char *g_current_path;

int update_handler(struct buffer *buf);

#endif
