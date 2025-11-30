#include "file_helpers.h"
#include "../logger/logger.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
signed int write_contents(const char *path, char *mode, void *data, size_t data_size, size_t type_size){


  char buff[256];
  snprintf(buff, sizeof(buff), "Writing contents to: %s - at mode %s - with data_siz: %ld - type_size: %ld", path, mode, data_size, type_size); 
  LOGGER_DEBUG(buff);

  FILE *fptr = fopen(path, mode);
  if(fptr == NULL){
    LOGGER_ERROR("Error opening the file for write operation.");
    return 1;
  }

  size_t wb = fwrite(data, type_size, data_size, fptr);
  if(wb != data_size){
    LOGGER_ERROR("Error writing to the file.");
    fclose(fptr);
    return 2;
  }

  fclose(fptr);
  return 0;
}
char *read_file_contents(const char *path){
  
  FILE *file_ptr;
  file_ptr = fopen(path, "rb");
  if(file_ptr == NULL){
    perror("read_file_contents");
    return NULL;
  }
  
  if(fseek(file_ptr, 0, SEEK_END) != 0) {
    perror("fseek error at read_file_contents");
    fclose(file_ptr);
    return NULL;
  }

  long size = ftell(file_ptr);
  if(size < 0) {
    perror("ftell size does not match");
    fclose(file_ptr);
    return NULL;
  }

  rewind(file_ptr);
  char *buffer = malloc((size_t)size + 1);
  if(buffer == NULL) {
    perror("Malloc failed at read file contents");
    fclose(file_ptr);
    return NULL;

  }

  size_t read_size = fread(buffer, 1, (size_t)size, file_ptr);
  if(read_size != (size_t)size) {
     fprintf(stderr, "Warning: only read %zu of %ld bytes\n", read_size, size);
     free(buffer);
     fclose(file_ptr);
     return NULL;
  }

  buffer[size] = '\0';
  fclose(file_ptr);
  return buffer;


}
