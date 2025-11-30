#include "gab/gab.h"
#include "gab_assembler/gab_assembler.h"
#include "helpers/file_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger/logger.h"



void usage(){
  printf("Usage: gab -r <source-path> | gab -c <source-path> -o <bin-path> | gab -e <bin-path>\n");
}

int main(int argc, char **argv){

  (void)argc; 
  (void)argv; 

  if(argc < 2){
    usage();
    return 1;
  }

  gab g = {0};
  
  if(argc == 3 && strcmp(argv[1], "-r") == 0) {
    const char *rom_path = argv[2];
    const char *src = read_file_contents(rom_path);
    gab_bin* bin = compile(src);
    load_rom(bin->data, &g);
    while(1){step(&g); }
    free(bin);
    return 0;
  }

  if(argc == 3 && strcmp(argv[1], "-e") == 0){
    
    const char *exec_path = argv[2];
    uint8_t *data = (uint8_t*)read_file_contents(exec_path);
    load_rom(data, &g);
    while(1){step(&g); }
    free(data);
    return 0;
    
  }

  if(argc == 5 && strcmp(argv[1], "-c") == 0 && strcmp(argv[3], "-o") == 0){
    const char *source_path = argv[2];
    const char *source_code = read_file_contents(source_path);
    gab_bin *bin = compile(source_code);
    const char *output = argv[4];
    signed int status = write_contents(output, "wb", (void*)bin->data, bin->size, sizeof(uint8_t));
    if(status == 0){
      LOGGER_SUCCESS("[COMPILATION] OK");
      return 0;
    }
    LOGGER_ERROR("[COMPILATION] ERROR");
    return 1;
  }
  
  usage();
  return 0;

}
