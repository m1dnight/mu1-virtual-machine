#include <inttypes.h>
#include <stdio.h>
#include "conditionals.h"

/*
Prints the condition bits.
*/
void
printConditions(uint8_t flags)
{
  printf("CONDITIONS:\n");
  printf("N: %" PRIu8 "\n", (uint8_t) (flags & SET_N_MASK));
  printf("Z: %" PRIu8 "\n", (uint8_t) (flags & SET_Z_MASK));
  printf("V: %" PRIu8 "\n", (uint8_t) (flags & SET_V_MASK));
  printf("C: %" PRIu8 "\n", (uint8_t) (flags & SET_C_MASK));

}
