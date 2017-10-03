#define MEMSIZE 128 // 256 words of memory (16 * 256 bits)

uint16_t
readSourceValue2Op(uint16_t *memory, uint16_t *registers, uint16_t word);

uint16_t
readDestinationValue2Op(uint16_t *memory, uint16_t *registers, uint16_t word);

void
writeDestinationValue2Op(uint16_t *memory, uint16_t *registers, uint16_t word, uint16_t new_value);

uint16_t*
initMemory();

void
printMemory(uint16_t *memory);
