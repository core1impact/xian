#ifndef FILE_H
#define FILE_H

#include <windows.h>
#include "../../buffer.h"

int write_file(const char *file_name, const struct buffer *buf, int flags);

int move_file(const char *file_name, const char *new_file_name);

#endif
