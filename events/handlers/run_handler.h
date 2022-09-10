#ifndef RUN_HANDLER_H
#define RUN_HANDLER_H

#include "../../common/buffer.h"
#include "../../config.h"
#include "../../modules/exec.h"
#include "../../transports/transport.h"

int run_handler(struct buffer *command);

#endif
