#include "process.h"

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
