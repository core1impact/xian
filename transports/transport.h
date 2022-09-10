#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "../config.h"
#include "curl_transport.h"

int transport_init();

void transport_destroy();

int (*transport_send)(const char *control, const struct buffer *data, struct buffer *command, struct buffer *params);

#endif
