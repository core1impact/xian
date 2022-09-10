#include "file.h"

int file_exists(const char *name) {
    struct stat buffer;
    return (stat(name, &buffer) == 0);
}

int copyFile(const char *source, const char *destination) {
    int input, output;
    if ((input = open(source, O_RDONLY)) == -1) {
        return -1;
    }
    if ((output = creat(destination, 0755)) == -1) {
        close(input);
        return -1;
    }

#if defined(__APPLE__) || defined(__FreeBSD__)
    int result = fcopyfile(input, output, 0, COPYFILE_ALL);
#else
    off_t bytesCopied = 0;
    struct stat fileinfo = {0};
    fstat(input, &fileinfo);
    int result = sendfile(output, input, &bytesCopied, fileinfo.st_size);
#endif

    close(input);
    close(output);

    return result;
}
