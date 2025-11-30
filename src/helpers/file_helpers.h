#ifndef FILE_HELPERS_H
#define FILE_HELPERS_H 

#include <stdlib.h>

char *read_file_contents(const char *path);
signed int write_contents(const char *path, char *mode, void *data, size_t data_size, size_t type_size);
#endif 
