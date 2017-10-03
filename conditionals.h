#define SET_N_MASK 0x01
#define SET_Z_MASK 0x02
#define SET_V_MASK 0x04
#define SET_C_MASK 0x08

#define set_n_bit(x)    (uint8_t) (x | SET_N_MASK)
#define set_z_bit(x)    (uint8_t) (x | SET_Z_MASK)
#define set_v_bit(x)    (uint8_t) (x | SET_V_MASK)
#define set_c_bit(x)    (uint8_t) (x | SET_C_MASK)

#define get_c_bit(x)    (uint8_t) (x & SET_C_MASK) >> 3


void
printConditions(uint8_t flags);
