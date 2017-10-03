#define MODE_SRC   0x0E00
#define MODE_DST   0x0038

#define IMMED      0x0
#define MODE1      0x1
#define MODE2      0x2

#define R_SRC      0x01C0
#define R_DST      0x0007

#define SINGLE_OP  0x0FFF

#define OP_MASK    0xF000




#define operation(x)    (uint16_t) ((x & OP_MASK) >> 12)
#define src_register(x) (uint16_t) ((x & R_SRC) >> 6)
#define dst_register(x) (uint16_t) ((x & R_DST))
#define src_mode(x)     (uint16_t) ((x & MODE_SRC) >> 9)
#define dst_mode(x)     (uint16_t) ((x & MODE_DST) >> 3)
#define single_op(x)    (uint16_t) ((x & SINGLE_OP))
#define get_word(x)     memory[x / 2];
