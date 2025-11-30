#ifndef GAB_ASSEMBLER_H 
#define GAB_ASSEMBLER_H 


#include <stdint.h>
#include <stdlib.h>

typedef struct { 
  uint8_t *data; 
  size_t size;

} gab_bin;

gab_bin * compile(const char *source);


#endif 
