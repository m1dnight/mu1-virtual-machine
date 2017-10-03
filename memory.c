#include <inttypes.h>
#include "defines.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint16_t*
initMemory()
{
  // We generate a sufficiently large enough chunk of memory.
  // A single word in the VM is 16 bits, so one word can be stored in a uint16_t.
  uint16_t  *memory = malloc(sizeof(uint16_t) * MEMSIZE);
  for (int i = 0; i < MEMSIZE; i++)
    memory[i] = 0;

  return memory;
}


/*
Expects a two-operand instruction word.
Reads out the source value of that word according to addressing mode and register.
*/
uint16_t
readSourceValue2Op(uint16_t *memory, uint16_t *registers, uint16_t word)
{
  uint16_t srcm = src_mode(word);
  uint16_t srcr = src_register(word);

  uint16_t src_value = 0x0;

  switch(srcm)
    {
    case IMMED:
      {
        src_value = registers[srcr - 1];
        break;
      }
    case MODE1:
      {
        src_value = memory[registers[srcr - 1]];
        break;
      }
    case MODE2:
      {
        // Mode 2 means autoincrement. The VM uses the value of the
        // register that was given as an address and uses that value.
        // The value of the register however, is incremented
        // afterwards!
        src_value = memory[registers[srcr - 1]];
        registers[srcr - 1] = registers[srcr - 1] + 1;
        break;
      }
    }
  return src_value;
}

uint16_t
readDestinationValue2Op(uint16_t *memory, uint16_t *registers, uint16_t word)
{
  const uint16_t dstm = dst_mode(word);
  const uint16_t dstr = dst_register(word);

  switch(dstm)
    {
    case IMMED:
      {
        return registers[dstr - 1];
        break;
      }
    case MODE1:
      {
        return memory[registers[dstr - 1]];
        break;
      }
    case MODE2:
      {
        return memory[registers[dstr - 1]];
        registers[dstr - 1] = registers[dstr - 1] + 1;
        break;
      }
    }
}


void
writeDestinationValue2Op(uint16_t *memory, uint16_t *registers, uint16_t word, uint16_t new_value)
{
  const uint16_t dstm = dst_mode(word);
  const uint16_t dstr = dst_register(word);
  switch(dstm)
    {
    case IMMED:
      {
        registers[dstr - 1] = new_value;
        break;
      }
    case MODE1:
      {
        memory[registers[dstr - 1]] = new_value;
        break;
      }
    case MODE2:
      {
        memory[registers[dstr - 1]] = new_value;
        break;
      }
    }
}
