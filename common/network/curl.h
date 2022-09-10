#ifndef CURL_H
#define CURL_H

#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "../../config.h"
#include "../buffer.h"

int curl_init();

void curl_destroy();

int curl_post(const char *url, const char **custom_headers, size_t custom_headers_size, const char *post_data,
              struct buffer *resp_headers, struct buffer *resp_body);

#endif
