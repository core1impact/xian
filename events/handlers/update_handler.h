#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#if defined(__linux__) || defined(__APPLE__)
#include "nix/update_handler.h"
#elif defined(__WIN32__)
#include "win/update_handler.h"
#endif

int update_handler(struct buffer *buf);

#endif
