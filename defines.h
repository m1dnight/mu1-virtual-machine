#define MEMSIZE 128 // 256 words of memory (16 * 256 bits)

#define MODE_SRC   0x0E00
#define MODE_DST   0x0038

#define IMMED      0x0
#define MODE1      0x1
#define MODE2      0x2

#define R_SRC      0x01C0
#define R_DST      0x0007

#define SINGLE_OP  0x0FFF

#define OP_MASK    0xF000

#define SET_N_MASK 0x01
#define SET_Z_MASK 0x02
#define SET_V_MASK 0x04
#define SET_C_MASK 0x08

#define set_n_bit(x)    (uint8_t) (x | SET_N_MASK)
#define set_z_bit(x)    (uint8_t) (x | SET_Z_MASK)
#define set_v_bit(x)    (uint8_t) (x | SET_V_MASK)
#define set_c_bit(x)    (uint8_t) (x | SET_C_MASK)

#define get_c_bit(x)    (uint8_t) (x & SET_C_MASK) >> 3

#define operation(x)    (uint16_t) ((x & OP_MASK) >> 12)
#define src_register(x) (uint16_t) ((x & R_SRC) >> 6)
#define dst_register(x) (uint16_t) ((x & R_DST))
#define src_mode(x)     (uint16_t) ((x & MODE_SRC) >> 9)
#define dst_mode(x)     (uint16_t) ((x & MODE_DST) >> 3)
#define single_op(x)    (uint16_t) ((x & SINGLE_OP))
#define get_word(x)     memory[x / 2];
