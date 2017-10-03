#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "defines.c"
/*
  Converts a uint16_t to big endian.
*/
uint16_t
toBE(uint16_t v)
{
  return (v << 8) | (v >> 8);
}

/*
  Converts a uint16_t to little endian.
*/
uint16_t
toLE(uint16_t v)
{
  return (v << 8) | (v >> 8);
}
