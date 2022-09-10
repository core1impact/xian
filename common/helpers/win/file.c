#include "file.h"

int write_file(const char *file_name, const struct buffer *buf, int flags) {
    HANDLE hFile = CreateFile(file_name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, flags, 0);

    if (INVALID_HANDLE_VALUE == hFile) {
        return 0;
    }

    DWORD dwTemp;

    BOOL bRet = WriteFile(hFile, buf->ptr, buf->size, &dwTemp, 0);

    int ret = bRet ? 1 : 0;
    CloseHandle(hFile);

    return ret;
}

int move_file(const char *file_name, const char *new_file_name) {
    BOOL bRet = MoveFile(file_name, new_file_name);

    return bRet ? 1 : 0;
}
