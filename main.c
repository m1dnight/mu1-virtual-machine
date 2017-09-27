#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

/*
Returns the number of bytes in the file.
 */
int
file_size(FILE * f)
{
	fseek(f, 0L, SEEK_END);
	int		sz = ftell(f);
	rewind(f);
	return sz;
}

uint16_t toBE(uint16_t v)
{
  return (v << 8) | (v >> 8);
}

uint16_t toLE(uint16_t v)
{
  return (v << 8) | (v >> 8);
}

int
main(int argc, char *argv[])
{

  
	//Read in the filename of the binary.
	char *filename = argv[1];
	printf("file: %s\n", filename);

	//Open the file.
	FILE * ptr;
	ptr = fopen(filename, "rb");

	//Determine file size and allocate buffer.
	int size = file_size(ptr) / sizeof(uint16_t); // size is in bytes!
        printf("Instruction count: %d\n", size);

	uint16_t buffer[size];

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
          printf("%" PRIu16 "\n", buffer[i]);
        }


        exit(1);
	fread(buffer, sizeof(buffer), 1, ptr);

	//Print out the binary(debugging)
	for (int i = 0; i < size; i+=2) {
		printf("0x%04x\n", buffer[i]);
	}
	printf("\n");

	//Setup the machine memory.
	unsigned char  *memory = malloc(sizeof(char) * 256);
	for (int i = 0; i < 256; i++) {
		memory[i] = 0x0;
	}

	//Copy the program into memory.
		for (int i = 0; i < size; i++) {
		memory[i] = buffer[i];
	}

	// //Internal state.
	// unsigned char	r1 = 0x0;
	// unsigned char	r2 = 0x0;
	// unsigned char	r3 = 0x0;
	// unsigned char	r4 = 0x0;
	// unsigned char	r5 = 0x0;
	//
	//
	// 	// Start parsing the file.
	// 	while (halt != 1) {
	// 	unsigned char	current = memory[PC];
	//
	// 	switch (current) {
	// 		unsigned char	s;
	// 		unsigned char	temp;
	//
	// 		//ACC <=[S]
	// 	case 0x0:
	// 		printf("\nACC <= [S]");
	// 		printf("\n s = 0x%x", memory[PC + 1]);
	// 		s = memory[PC + 1];
	// 		acc = memory[s];
	// 		PC++;
	// 		PC++;
	// 		break;
	//
	// 		//ACC = >[S]
	// 	case 0x1:
	// 		printf("\nACC => [S]");
	// 		s = memory[PC + 1];
	// 		printf("\n  Writing 0x%x to address 0x%x", acc, s);
	// 		memory[s] = acc;
	// 		PC++;
	// 		PC++;
	// 		break;
	//
	// 		//ACC +[S]
	// 	case 0x2:
	// 		printf("\nACC + [S]");
	// 		s = memory[PC + 1];
	// 		temp = memory[s];
	// 		acc = acc + temp;
	// 		PC++;
	// 		PC++;
	// 		break;
	//
	// 		//ACC -[S]
	// 	case 0x3:
	// 		printf("\nACC - [S]");
	// 		s = memory[PC + 1];
	// 		temp = memory[s];
	// 		acc = acc - temp;
	// 		PC++;
	// 		PC++;
	// 		break;
	//
	//
	// 		//PC <= S
	// 	case 0x4:
	// 		printf("\nPC <= S");
	// 		s = memory[PC + 1];
	// 		PC = s;
	// 		break;
	//
	// 		//IF + VE PC <= S
	// 	case 0x5:
	// 		printf("\nIF +VE PC <= S");
	// 		s = memory[PC + 1];
	// 		if (acc > 0x0) {
	// 			PC = s;
	// 		}
	// 		PC++;
	// 		PC++;
	// 		break;
	//
	// 		//IF != 0 PC <= S
	// 	case 0x6:
	// 		printf("\nIF != 0 PC <= S");
	// 		s = memory[PC + 1];
	// 		if (acc == 0x0) {
	// 			PC = s;
	// 		}
	// 		PC++;
	// 		PC++;
	// 		break;
	// 	case 0x7:
	// 		printf("\nHALT");
	// 		halt = 1;
	// 		break;
	//
	// 	}
	//
	// 	// Print out new state.
	// 	printf("\n ACC: 0x%x", acc);
	// 	printf("\n PC : 0x%x", PC);
	// 	printf("\nMemory:\n");
	// 	for (int i = 0; i < 32; i++) {
	// 		printf("0x%x ", memory[i]);
	// 	}
	// 	printf("\n");
	// }
	return 1;
}
