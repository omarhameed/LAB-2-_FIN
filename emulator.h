#ifndef EMULATOR_H
#define EMULATOR_H
#define MEM_SIZE 0x10000
#define MAXBufSize 256
#define number_reg 8
#define num_consts 8
#define MEM_SIZE 0x10000
#define MAXBufSize 256
#define WORD_MEM_SIZE 1<<15
#define BYTE_MEM_SIZE 1<<16
//#define Hex_2_Bit(var, pos) (((var) >> (pos)) & 1)
//#define Hex_2_Bit(var, pos) (((var) & (1 << pos))>>pos)
#define Hex_2_Bit(src, bit) ((src >> bit) & 1)
//#define Hex_2_Bit(src, bit) (((src) >> ((sizeof(src) * 8) - 1 - (bit))) & 1)

//#define Hex_2_Bit(var, pos) (((var) & (1 << (pos))) != 0)
// Masking all bits before encoded offset 
//#define  Maskbits(bits)  (bits & 0x3FF) 
#define  Maskbits(bits)  (bits & 0x03FF) 

#define SET_BITS(hexValue, x, value) (((hexValue) & (~((1 << (16 - (x))) - 1))) | ((value) ? ((1 << (16 - (x))) - 1) : 0))
#define MASK_BITS(x, from, to) ((x) & (~(((1 << ((to) - (from) + 1)) - 1) << (from))))

#define HIGH_BYTE_SHIFT(x) (x*(1 << 8))	// returns the value of x shfited by 8 bits


#define HEX_SIZE 8
#define BIT_ARRAY_SIZE 16//(HEX_SIZE * 4)
extern union Memory {
    unsigned short WordMem[WORD_MEM_SIZE];
    unsigned char ByteMem[BYTE_MEM_SIZE];
}memory_u;
//  00000000 1101000

extern void Bus(unsigned short mar, unsigned short* mdr_ptr, int read_write, int word_byte);
extern void Fetch_Instr();
extern void Control();
extern void Decode(int* bit_array);
extern unsigned char testing_input[MAXBufSize];
extern unsigned char memory[MEM_SIZE];
extern unsigned short RegFile[2][num_consts];
extern unsigned short instr_reg;
extern void hexToBitArray(unsigned int hexValue, int* bitArray);
extern void Branching();
extern void Arithmentic();
extern void printBits(instr_reg);
    // Allocate 65,536 bytes of memory

#endif
