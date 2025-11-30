#include "gab_assembler.h"
#include "../logger/logger.h"
#include "../gab/gab.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void swap_str(char *src, char *dst, size_t start, size_t up_to){

  size_t idx = 0;
  for(size_t k = start; k <= up_to; k++) {
    if(dst[idx] == '\n'){
      dst[idx] = '\0';
      return; 
    }

    dst[idx] = src[k];
    idx++;
  }
  dst[idx] = '\0';
}

#define INS_TO_GAB_ISA_CHECK(str_var, instr, out_var) \
  do {                                                \
    if(strcmp(str_var, #instr) == 0) {                \
      *out_var = instr;                                                \
      return;                                         \
    }                                                 \
  } while(0)

void instr_to_gab_isa(char *instr, gab_isa* out){
  
  if(instr == NULL) return;
  (void)out; 
  INS_TO_GAB_ISA_CHECK(instr, LOD, out);
  INS_TO_GAB_ISA_CHECK(instr, STR, out);
  INS_TO_GAB_ISA_CHECK(instr, MOV, out);
  INS_TO_GAB_ISA_CHECK(instr, LDI, out);
  INS_TO_GAB_ISA_CHECK(instr, ADD, out);
  INS_TO_GAB_ISA_CHECK(instr, INC, out);
  INS_TO_GAB_ISA_CHECK(instr, DEC, out);
  INS_TO_GAB_ISA_CHECK(instr, OUT, out);
  INS_TO_GAB_ISA_CHECK(instr, HLT, out);
}

uint8_t str_to_reghex(char *reg_str){
  if(reg_str == NULL) return 0x05; // there's no 5-th reg 
  if(strcmp(reg_str, "R0") == 0) return 0x0;
  if(strcmp(reg_str, "R1") == 0) return 0x1;
  if(strcmp(reg_str, "R2") == 0) return 0x2;
  if(strcmp(reg_str, "R3") == 0) return 0x3;
  if(strcmp(reg_str, "R4") == 0) return 0x4;
  return 0x05;

}



gab_bin* compile(const char *source){

  if(source == NULL) return NULL;
  char *cp = strdup(source);
  const char *delimiter = "\n";
  char *line = strtok(cp, delimiter);
  
  char instr[4];
  char reg[3];
  char val[5];

  size_t sl = strlen(source);

  gab_bin *bin = malloc(sizeof(gab_bin));
  uint8_t *data = malloc(sizeof(char) * sl);
  if(bin == NULL || data == NULL){
    LOGGER_ERROR("Error allocating memory for binary.");
    return NULL;
  }
  size_t ic = 0;
  while(line != NULL){
    gab_isa instr_e;
    char buff[256];
    snprintf(buff, sizeof(buff), "[INSTRUCTION]: %s", line);
    LOGGER_DEBUG(buff);



    if(strlen(line) < 3) {
      LOGGER_ERROR("Line doesn't have enough tokens.");
      return NULL;
    }
   
    swap_str(line, instr, 0, 2);
    LOGGER_SUCCESS("[INSTR] OK");
    swap_str(line, reg, 4, 5);
    LOGGER_SUCCESS("[REG] OK");
   
    instr_to_gab_isa(instr, &instr_e);
    if(instr_e == HLT){
      data[ic++] = HLT << 4;
      line = strtok(NULL, delimiter);
      continue;
    }
    
    uint8_t regist_val = str_to_reghex(reg);
    if(regist_val == 0x05) {
      LOGGER_ERROR("Invalid register value provided in source code.");
      return NULL;
    }
    
    if(instr_e == INC || instr_e == DEC || instr_e == OUT){
      data[ic++] = (instr_e << 4) | regist_val;
      line = strtok(NULL, delimiter);
      continue;
    }

  
    swap_str(line, val, 8, 11);
    long v = strtol(val, NULL, 10);
    if(v > 255) v = 255; 
    data[ic++] = (instr_e << 4) | regist_val;
    data[ic++] = v;
    line = strtok(NULL, delimiter);
  }

  LOGGER_SUCCESS("[BIN] OK");
  free(cp);
  bin->data = data;
  bin->size = ic;
  return bin;

}
