#ifndef EXEC_H
#define EXEC_H

#include <stdio.h>
#include <string.h>

#include "../config.h"
#include "../common/buffer.h"

int exec(const char *command, struct buffer *buf);

#endif
