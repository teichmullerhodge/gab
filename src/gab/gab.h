#ifndef GAB_H 
#define GAB_H 

#include <stdint.h>
#define DBG_MODE 0 



typedef enum {

  LOD = 0x0, 
  STR = 0x1, 
  MOV = 0x2,
  LDI = 0x3,
  ADD = 0x4,
  SUB = 0x5,
  INC = 0x6, // ok 
  DEC = 0x7, // ok 
  OUT = 0x8, // ok  
  HLT = 0x9, // ok 

  // LOD R0, 10

} gab_isa;

typedef struct {

  uint8_t registers[4];
  uint8_t pc;
  uint8_t sp;
  uint8_t zf;
  uint8_t memory[1 << 8];

} gab;




void load_rom(uint8_t *rom, gab* gab);
void step(gab* gab);


#endif 
