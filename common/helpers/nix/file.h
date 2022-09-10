#ifndef FILE_H
#define FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <copyfile.h>
#endif

int file_exists(const char *name);

#endif