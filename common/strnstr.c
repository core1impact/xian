#include "strnstr.h"

char *strnstr(const char *haystack, const char *needle, size_t len) {
    size_t needle_len = strlen(needle);

    if (needle_len > len) {
        return 0;
    }

    for (size_t i = 0; i <= (len - needle_len); ++i) {
        if (!strncmp(haystack, needle, needle_len)) {
            return (char *) haystack;
        }
        haystack++;
    }

    return 0;
}
