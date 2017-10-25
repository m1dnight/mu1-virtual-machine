/****************************/
/* Two-Operand Instructions */
/****************************/

// Mode identifiers.
#define IMMED      0x0
#define MODE1      0x1
#define MODE2      0x2

// Register masks.
#define R_SRC      0x01C0
#define R_DST      0x0007

// Operation mask.
#define OP_MASK    0xF000

// Mode masks.
#define MODE_SRC   0x0E00
#define MODE_DST   0x0038

#define operation(x)    (uint16_t) ((x & OP_MASK) >> 12)

// Register operand macros.
#define src_register(x) (uint16_t) ((x & R_SRC) >> 6)
#define dst_register(x) (uint16_t) ((x & R_DST))

// Register operand mode macro.
#define src_mode(x)     (uint16_t) ((x & MODE_SRC) >> 9)
#define dst_mode(x)     (uint16_t) ((x & MODE_DST) >> 3)

// Mode masks.

/****************************/
/* One-Operand Instructions */
/****************************/
#define SINGLE_OP  0x0FFF

#define single_op_operand(x)    (uint16_t) ((x & SINGLE_OP))


/*************************/
/* Operation Identifiers */
/*************************/
#define MOV  0x0
#define ADD  0x1
#define SUB  0x2
#define CMP  0x3
#define BEQ  0x4
#define HALT 0x5
#define BNE  0x6
#define JMP  0x7
