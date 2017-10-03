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
