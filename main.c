#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define MEMSIZE 128 // 256 words of memory (16 * 256 bits)

// Addressing mode masks.
#define MODE_SRC  0x0E00
#define MODE0_SRC 0x0000
#define MODE1_SRC 0x0200
#define MODE2_SRC 0x0400
#define MODE3_SRC 0x0600

#define MODE_DST  0x0038
#define MODE0_DST 0x0000
#define MODE1_DST 0x0008
#define MODE2_DST 0x0010
#define MODE3_DST 0x0018

#define IMMED 0x0
#define MODE1 0x1
#define MODE2 0x2

#define src_register(x) (uint16_t) ((x & R_SRC) >> 6)
#define dst_register(x) (uint16_t) ((x & R_DST))
#define src_mode(x)     (uint16_t) ((x & MODE_SRC) >> 9)
#define dst_mode(x)     (uint16_t) ((x & MODE_DST) >> 3)


// Register masks.
#define R_SRC  0x01C0
#define R1_SRC 0x0040
#define R2_SRC 0x0080
#define R3_SRC 0x00C0
#define R4_SRC 0x0100
#define PC_SRC 0x0140

// Register masks for destination
#define R_DST  0x0007
#define R1_DST 0x0001 
#define R2_DST 0x0002 
#define R3_DST 0x0003  
#define R4_DST 0x0004
#define PC_DST 0x0005

#define OP_MASK 0xF000
#define MOV     0x000F


#define SET_N_MASK 0x1
#define SET_Z_MASK 0x2
#define SET_V_MASK 0x4
#define SET_C_MASK 0x8

#define set_n_bit(x) (uint8_t) (x | SET_N_MARK)
#define set_z_bit(x) (uint8_t) (x | SET_Z_MARK)
#define set_v_bit(x) (uint8_t) (x | SET_V_MARK)
#define set_c_bit(x) (uint8_t) (x | SET_C_MARK)

#define operation(x) (uint16_t) ((x & OP_MASK) >> 12)


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


/* 
   Reads an operand from memory depending on addressing mode and register values.
*/
uint16_t
readValue(uint16_t *memory, uint16_t *registers, uint16_t regist, uint16_t mode)
{
  switch(mode)
    {
      // Immediate mode (use the value from the register).
    case 0x0:
      return registers[regist];
      break;
      // Deferred mode (use the value of the register as the address in memory).
    case 0x1:
      return memory[registers[regist]];
      break;
      // Auto-increment: Use the value from the register as an address, and increment the register to the next address.
    case 0x2:
      {
        uint16_t regval    = registers[regist];
        uint16_t regvalnew = regval + 2;
        uint16_t operand   = memory[regval];
        registers[regist] = regvalnew;
        return operand;
        break;
      }
      
    default:
      fprintf(stderr, "Invalid addressing mode!\n");
      exit(0);
      break;
    }
  
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
  

  int stop = 4;
  
  while(stop > 0)
    {
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

      // Increment program counter.
      registers[4] = registers[4] + 1;
      
      switch(oper)
        {
          // MOV
        case 0x0:
          {
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
            stop -= 1;

            break;
          }

          // ADD src dst
        case 0x1:
          {
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
            stop -= 1;

            break;            
          }

          // SUB src dst
        case 0x2:
          {
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

            // Set the bits.
            
            stop -= 1;

            break;            
          }

          // CMP src dst
        case 0x3:
          {
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
            
            stop -= 1;

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
  return 0;
}



  
