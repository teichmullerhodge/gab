#include "gab.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../logger/logger.h"

void load_rom(uint8_t *rom, gab *gab){

  memcpy(gab->memory, rom, sizeof(gab->memory));

}

void step(gab *gab){

  if(gab == NULL) exit(2);


  uint8_t opcode = gab->memory[gab->pc++];
  uint8_t instruction = opcode >> 4;
  uint8_t r = opcode & 0x0F;

  #if DBG_MODE
    printf("OPCODE: %d | instruction: %d | register: %d\n", opcode, instruction, r); 
  #endif

  switch(instruction){
    case LOD:
    {
      uint8_t addr = gab->memory[gab->pc++];
      gab->registers[r] = gab->memory[addr];
      gab->zf = gab->registers[r] == 0;
      break;
    }
    case STR:
    {
      uint8_t addr = gab->memory[gab->pc++];
      gab->memory[addr] = gab->registers[r];
      gab->zf = gab->memory[addr] == 0;
      break;
    }
    
    case MOV:
    {
      uint8_t reg = gab->memory[gab->pc++];
      gab->registers[r] = gab->registers[reg];
      gab->zf = gab->registers[r] == 0;
      break;
    }

    case LDI:
    {
      uint8_t imm = gab->memory[gab->pc++];
      gab->registers[r] = imm;
      gab->zf = imm == 0;
      break;
    }

    case ADD:
    {
      uint8_t val = gab->memory[gab->pc++];
      gab->registers[r] += val; 
      gab->zf = gab->registers[r] == 0;
      break; 
    }
    case SUB:
    {
      uint8_t val = gab->memory[gab->pc++];
      gab->registers[r] -= val; 
      gab->zf = gab->registers[r] == 0;
      break; 
    }

    case INC:
    {
      gab->registers[r]++; 
      gab->zf = gab->registers[r] == 0;
      break; 
    }
    case DEC:
    {
      gab->registers[r]--; 
      gab->zf = gab->registers[r] == 0;
      break; 
    }
    case OUT:
    {
      printf("OUT: %d\n", gab->registers[r]);
      break;
    }

    case HLT: exit(1);

    default: {
      char buff[256];
      snprintf(buff, sizeof(buff), "Unknown instruction: %#X", instruction);
      LOGGER_ERROR(buff);
      exit(3);
    }
  }
}
