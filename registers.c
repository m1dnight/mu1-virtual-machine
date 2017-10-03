#include <inttypes.h>
#include <stdio.h>
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
