#ifndef CURL_TRANSPORT_H
#define CURL_TRANSPORT_H

#include "../common/base64.h"
#include "../common/network/curl.h"
#include "../common/strnstr.h"

#include "../modules/sysinfo.h" // FIXME

G_STR(server);

extern struct system_info g_sysinfo;

int curl_transport_init();

void curl_transport_destroy();

int
curl_transport_send(const char *control, const struct buffer *data, struct buffer *command, struct buffer *params);

#endif
