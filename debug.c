#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "defines.h"
/*
  Dumps the memory in a human readable way on stdout.
*/
void
printMemory(uint16_t *memory)
{
  printf("MEMDUMP:\n");
  // PRint out the memory.
  for(int i = 0; i < MEMSIZE / 8; i++)
    {
      printf("%3d - %3d: ", i * 8, i*8 + 7);
      for(int j = 0; j < 8; j++)
        {
          printf("|%16" PRIu16 " ", memory[(i * 8) + j]);
        }
      printf("|\n");
    }
}

/*
  Prints out the register values.
*/
void
printRegisters(uint16_t *registers)
{
  printf("REGISTERS:\n");
  printf("R1: %016" PRIu16 "\n", registers[0]);
  printf("R2: %016" PRIu16 "\n", registers[1]);
  printf("R3: %016" PRIu16 "\n", registers[2]);
  printf("R4: %016" PRIu16 "\n", registers[3]);
  printf("PC: %016" PRIu16 "\n", registers[4]);
}
