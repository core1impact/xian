#ifndef REGISTRY_H
#define REGISTRY_H

#include <windows.h>

int create_reg_item(HKEY hKey, char *key_path, char *key_name, char *key_value);

#endif
