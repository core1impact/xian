#ifndef UPDATE_HANDLER_NIX_H
#define UPDATE_HANDLER_NIX_H

#include <stdio.h>
#include "../../../config.h"
#include "../../../common/buffer.h"
#include "../../../common/helpers/nix/file.h"
#include "exec_handler.h"

G_STR(nix_bin_path);

int update_handler(struct buffer *buf);

#endif
