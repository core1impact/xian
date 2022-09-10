#include "curl.h"

int curl_init() {
    return curl_global_init(CURL_GLOBAL_DEFAULT) ? 0 : 1;
}

void curl_destroy() {
    curl_global_cleanup();
}

size_t null_buffer_callback(const char *data, size_t size, size_t block, void *out) {
    return size * block;
}

size_t buffer_callback(const char *data, size_t size, size_t block, struct buffer *out) {
    size_t size_block = size * block;

    if (size_block > CURL_BLOCK_SIZE_MAX) {
        goto clean;
    }

    size_t new_len = out->size + size_block;

    if (new_len > CURL_BUFFER_SIZE_MAX) {
        goto clean;
    }

    if (!out->ptr) {
        out->ptr = malloc(new_len);

        if (!out->ptr) {
            goto clean;
        }
    } else {
        char *temp_ptr = realloc(out->ptr, new_len);

        if (!temp_ptr) {
            goto clean;
        }
        out->ptr = temp_ptr;
    }

    memcpy(out->ptr + out->size, data, size_block);
    out->size = new_len;

    return size_block;

    clean:

    if (out->size) {
        free(out->ptr);
    }

    return 0;
}

int curl_post(const char *url, const char **custom_headers, size_t custom_headers_size, const char *post_data,
              struct buffer *resp_headers, struct buffer *resp_body) {
    CURL *curl = curl_easy_init();

    int ret = 0;

    if (!curl) {
        return ret;
    }

    struct curl_slist *slist = 0;

    if (custom_headers_size) {
        for (size_t i = 0; i < custom_headers_size; ++i) {
            struct curl_slist *slist_temp = curl_slist_append(slist, custom_headers[i]);

            if (!slist_temp) {
                goto clean;
            }
            slist = slist_temp;
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.1.4322)");

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

    if (resp_headers) {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, buffer_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, resp_headers);
    }

    if (resp_body) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, buffer_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp_body);
    } else {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, null_buffer_callback);
    }

    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
        ret = 1;
    }

    clean:

    if (slist) {
        curl_slist_free_all(slist);
    }

    curl_easy_cleanup(curl);

    return ret;
}
