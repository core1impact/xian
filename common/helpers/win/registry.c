#include "registry.h"

int create_reg_item(HKEY hKey, char *key_path, char *key_name, char *key_value) {
    LSTATUS ls = RegOpenKeyEx(hKey, key_path, 0, KEY_WRITE, &hKey);

    if (ls) {
        return 0;
    }

    ls = RegSetValueEx(hKey, key_name, 0, REG_SZ, key_value, strlen(key_value));

    RegCloseKey(hKey);

    return ls ? 0 : 1;
}
