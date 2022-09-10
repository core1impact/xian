#include "transport.h"

int transport_init() {
    if (TRANSPORT == CURL_TRANSPORT) {
        transport_send = curl_transport_send;
        return curl_transport_init();
    }
}

void transport_destroy() {
    if (TRANSPORT == CURL_TRANSPORT) {
        return curl_destroy();
    }
}
