#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "conditionals.h"
#include "debug.h"
#include "endianness.h"
#include "defines.h"
#include "memory.h"


/*
  Returns the number of bytes in a file.
*/
int
file_size(FILE * f)
{
  fseek(f, 0L, SEEK_END);
  int		sz = ftell(f);
  rewind(f);
  return sz;
}

int
main(int argc, char *argv[])
{
  /**********/
  /* SETUP  */
  /**********/

  // Read in the binary file and store it in an appropriately sized buffer.
  char *filename = argv[1];
  FILE * ptr     = fopen(filename, "rb");
  int size       = file_size(ptr) / sizeof(uint16_t); // size is in bytes!
  uint16_t buffer[size];

  for(int i = 0; i < size; i++)
    {
      buffer[i] = 0;
    }

  fread(buffer, sizeof(uint16_t), size, ptr);

  // The assembler generates big endian values. C requires us to use little
  // endian integers so we convert all the entries in the program.
  for(int i = 0; i < size; i++)
      buffer[i] = toLE(buffer[i]);

  uint16_t *memory = initMemory();

  // Finally we copy the program into memory. The program starts at the from of
  // the chunk of memory.
  for (int i = 0; i < size; i++)
    memory[i] = buffer[i];

  /*********/
  /* START */
  /*********/

  // MU1 has five registers. R1 to R4 and PC being the fifth.
  // Additionally we also have four conditional flags. These are only four bits
  // so we store them in a uint8, which is the smallest we can use.
  uint16_t registers[5] = {0x0, 0x0, 0x0, 0x0, 0x0};
  uint8_t  flags        = 0x0;

  // The stop flag is put to 1 if we encounted the STOP instruction. Otherwise
  // the VM keeps incrementing the program counter and evaluating
  // instructions found at that memory address.
  int stop = 0;
  while(stop != 1)
    {
      const uint16_t word = memory[registers[4]];
      const uint16_t oper = operation(word);

      // Increment program counter. In the theoretical model the PC goes forward
      // in steps of 2. We have one word per line, so we increment by 1. The
      // below expression also means that every expression is evaluated with the
      // program counter already pointing to the *next* instruction!
      registers[4] = registers[4] + 1;

      switch(oper)
        {
          // MOV
        case 0x0:
          {
            uint16_t src_value = readSourceValue2Op(memory, registers, word);
            writeDestinationValue2Op(memory, registers, word, src_value);
            break;
          }

          // ADD src dst
        case 0x1:
          {
            uint16_t src_value = readSourceValue2Op(memory, registers, word);
            uint16_t dst_value = readDestinationValue2Op(memory, registers, word);
            writeDestinationValue2Op(memory, registers, word, src_value + dst_value);
            break;
          }

          // SUB src dst
        case 0x2:
          {
            uint16_t src_value = readSourceValue2Op(memory, registers, word);
            uint16_t dst_value = readDestinationValue2Op(memory, registers, word);
            writeDestinationValue2Op(memory, registers, word, dst_value - src_value);
            break;
          }

          // CMP src dst
        case 0x3:
          {
            uint16_t dst_value = readDestinationValue2Op(memory, registers, word);
            uint16_t src_value = readSourceValue2Op(memory, registers, word);

            if(src_value - dst_value == 0)
              {
                flags = set_c_bit(flags);
              }
            break;
          }

          // BEQ offset
          // Note: The offset is an 8 bit 2c number!!
        case 0x4:
        {
          // Op contains the offset in 2C but in 16 bits.
          int8_t   offset = ((int8_t) single_op(word)) / 2 - 1; // -1 for the PC pointing to the next line already.
          uint16_t target = registers[4] + offset;

          if(get_c_bit(flags) == 1)
          {
            registers[4] = target;
          }
          break;
        }

          // HALT
        case 0x5:
          {
            stop = 1;
            break;
          }

          // BNE offset
          // Note: The offset is an 8 bit 2c number!!
        case 0x6:
          {
            // Op contains the offset in 2C but in 16 bits.
            int8_t   offset = ((int8_t) single_op(word)) / 2 - 1; // -1 for the PC pointing to the next line already.
            uint16_t target = registers[4] + offset;

            if(get_c_bit(flags) != 1)
            {
              registers[4] = target;
            }
            break;
          }
        default:
          {
            exit(0);
            break;
          }
        }
    }
  printMemory(memory);
  printRegisters(registers);
  printConditions(flags);
  return 0;
}
