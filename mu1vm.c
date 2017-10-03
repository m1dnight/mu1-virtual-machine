#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "debug.h"
#include "endianness.h"
#include "defines.c"


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

  //Read in the filename of the binary.
  char *filename = argv[1];

  //Open the file.
  FILE * ptr;
  ptr = fopen(filename, "rb");

  //Determine file size and allocate buffer.
  int size = file_size(ptr) / sizeof(uint16_t); // size is in bytes!

  uint16_t buffer[size];

  // Init with zero.
  for(int i = 0; i < size; i++)
    {
      buffer[i] = 0;
    }

  // Read in the contents of the file in the buffer.
  fread(buffer, sizeof(uint16_t), size, ptr);

  // The binary files are big endian, convert all the values to little.
  for(int i = 0; i < size; i++)
    {
      buffer[i] = toLE(buffer[i]);
    }

  //Setup the machine memory.
  uint16_t  *memory = malloc(sizeof(uint16_t) * MEMSIZE);
  for (int i = 0; i < MEMSIZE; i++) {
    memory[i] = 0;
  }

  //Copy the program into memory.
  for (int i = 0; i < size; i++) {
    memory[i] = buffer[i];
  }

  /*********/
  /* START */
  /*********/

  // R1 R2, R3, R4, PC
  uint16_t registers[5] = {0x0, 0x0, 0x0, 0x0, 0x0};
  uint8_t  flags        = 0x0;


  int stop = 0;

  while(stop != 1)
    {
      //getchar( );
      printf("--------------------------------------\n");
      const uint16_t word = memory[registers[4]];
      printf("Current word: %" PRIu16 "\n", word);

      const uint16_t oper = operation(word);
      printf(" - operation: %" PRIu16 "\n", oper);

      const uint16_t srcm = src_mode(word);
      printf(" - src_mode : %" PRIu16 "\n", srcm);

      const uint16_t srcr = src_register(word);
      printf(" - src_reg  : %" PRIu16 "\n", srcr);

      const uint16_t dstm = dst_mode(word);
      printf(" - dst_mode : %" PRIu16 "\n", dstm);

      const uint16_t dstr = dst_register(word);
      printf(" - dst_reg  : %" PRIu16 "\n", dstr);

      printRegisters(registers);

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
            printf(" => MOV\n");
            const uint16_t srcm = src_mode(word);
            const uint16_t srcr = src_register(word);
            const uint16_t dstm = dst_mode(word);
            const uint16_t dstr = dst_register(word);

            uint16_t src_value = 0x0;

            // Read out the value to move to the destination.
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
            // Determine the address to move to.
            switch(dstm)
              {
              case IMMED:
                {
                  registers[dstr - 1] = src_value;
                  break;
                }
              case MODE1:
                {
                  memory[registers[dstr - 1]] = src_value;
                  break;
                }
              case MODE2:
                {
                  memory[registers[dstr - 1]] = src_value;
                  registers[dstr - 1] = registers[dstr - 1] + 1;
                  break;
                }
              }
            break;
          }

          // ADD src dst
        case 0x1:
          {
            printf(" => ADD\n");
            const uint16_t srcm = src_mode(word);
            const uint16_t srcr = src_register(word);
            const uint16_t dstm = dst_mode(word);
            const uint16_t dstr = dst_register(word);

            uint16_t src_value = 0x0;

            // Read out the value to add to the destination.
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
                  src_value = memory[registers[srcr - 1]];
                  registers[srcr - 1] = registers[srcr - 1] + 1;
                  break;
                }
              }
            // Determine the address to move to.
            switch(dstm)
              {
              case IMMED:
                {
                  registers[dstr - 1] = registers[dstr-1] + src_value;
                  break;
                }
              case MODE1:
                {
                  memory[registers[dstr - 1]] = memory[registers[dstr - 1]] + src_value;
                  break;
                }
              case MODE2:
                {
                  memory[registers[dstr - 1]] = memory[registers[dstr - 1]] + src_value;
                  registers[dstr - 1] = registers[dstr - 1] + 1;
                  break;
                }
              }
            break;
          }

          // SUB src dst
        case 0x2:
          {
            printf(" => SUB\n");
            const uint16_t srcm = src_mode(word);
            const uint16_t srcr = src_register(word);
            const uint16_t dstm = dst_mode(word);
            const uint16_t dstr = dst_register(word);

            uint16_t src_value = 0x0;

            // Read out the value to add to the destination.
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
                  src_value = memory[registers[srcr - 1]];
                  registers[srcr - 1] = registers[srcr - 1] + 1;
                  break;
                }
              }

            // Determine the address to move to.
            switch(dstm)
              {
              case IMMED:
                {
                  printf("Destination value: %" PRIu16 "\n", registers[dstr-1]);
                  registers[dstr - 1] = registers[dstr-1] - src_value;
                  break;
                }
              case MODE1:
                {
                  memory[registers[dstr - 1]] = memory[registers[dstr - 1]] - src_value;
                  break;
                }
              case MODE2:
                {
                  memory[registers[dstr - 1]] = memory[registers[dstr - 1]] - src_value;
                  registers[dstr - 1] = registers[dstr - 1] + 1;
                  break;
                }
              }

            break;
          }

          // CMP src dst
        case 0x3:
          {
            printf(" => CMP\n");
            const uint16_t srcm = src_mode(word);
            const uint16_t srcr = src_register(word);
            const uint16_t dstm = dst_mode(word);
            const uint16_t dstr = dst_register(word);

            uint16_t src_value = 0x0;
            uint16_t dst_value = 0x0;

            // Read out the value to add to the destination.
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
                  src_value = memory[registers[srcr - 1]];
                  registers[srcr - 1] = registers[srcr - 1] + 1;
                  break;
                }
              }

            switch(dstm)
              {
              case IMMED:
                {
                  dst_value = registers[dstr - 1];
                  break;
                }
              case MODE1:
                {
                  dst_value = memory[registers[dstr - 1]];
                  break;
                }
              case MODE2:
                {
                  dst_value = memory[registers[dstr - 1]];
                  registers[dstr - 1] = registers[dstr - 1] + 1;
                  break;
                }
              }

            uint16_t diff = src_value - dst_value;
            printf("CMP %" PRIu16 " - %" PRIu16 " = %" PRIu16 "\n", src_value, dst_value, diff);
            if(diff == 0)
              {
                flags = set_c_bit(flags);
              }
            break;
          }
          // BEQ offset
          // Note: The offset is an 8 bit 2c number!!
        case 0x4:
        {
          printf(" => BEQ\n");
          // Op contains the offset in 2C but in 16 bits.
          uint16_t op16 = single_op(word);
          uint8_t  op8  = (uint8_t) op16;
          int8_t   op8s = op8;
          int8_t   offset = op8s / 2 - 1; // -1 for the PC pointing to the next line already.
          uint16_t target = registers[4] + offset;
          printf("The offset is %" PRId8 "\n", offset);
          printf("Current PC: %" PRIu16 "\n", registers[4]);
          printf("Jumping to: %" PRIu16 "\n", target);

          if(get_c_bit(flags) == 1)
          {
            printf("Jumping back to %" PRIu16 " (%" PRIu16 " + %" PRId8 ")\n", target, registers[4], offset);
            registers[4] = target;
          }
          else
          {
            printf("Comparison bit was not set. Not jumping.");
          }
          break;
        }
          // HALT
        case 0x5:
          {
            printf(" => HALT\n");
            stop = 1;
            break;
          }
          // BNE offset
          // Note: The offset is an 8 bit 2c number!!
        case 0x6:
          {
            printf(" => BNE\n");
            // Op contains the offset in 2C but in 16 bits.
            uint16_t op16 = single_op(word);
            uint8_t  op8  = (uint8_t) op16;
            int8_t   op8s = op8;
            int8_t   offset = op8s / 2 - 1; // -1 for the PC pointing to the next line already.
            uint16_t target = registers[4] + offset;
            printf("The offset is %" PRId8 "\n", offset);
            printf("Current PC: %" PRIu16 "\n", registers[4]);
            printf("Jumping to: %" PRIu16 "\n", target);

            if(get_c_bit(flags) != 1)
            {
              printf("Jumping back to %" PRIu16 " (%" PRIu16 " + %" PRId8 ")\n", target, registers[4], offset);
              registers[4] = target;
            }
            else
            {
              printf("Comparison bit was not set. Not jumping.");
            }
            break;
          }
        default:
          {

            fprintf(stderr, "Invalid instruction: %" PRIu16 "\n", oper);
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