#ifndef EVENTS_H
#define EVENTS_H

#include <unistd.h>

#include "../config.h"
#include "../common/buffer.h"
#include "../transports/transport.h"
#include "../modules/sysinfo.h"

#include "handlers/run_handler.h"

#if defined(__linux__) || defined(__APPLE__)

#include "handlers/nix/exec_handler.h"
#include "handlers/nix/update_handler.h"

#elif defined(__WIN32__)

#include "handlers/win/exec_handler.h"
#include "handlers/win/update_handler.h"

#endif

void event_loop();

#endif
