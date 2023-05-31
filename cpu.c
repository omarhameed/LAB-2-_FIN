#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "emulator.h"
//#define DEBUG
unsigned short RegFile[2][num_consts] = {
    {0xF0A0, 0xF1A1, 0xF2A2, 0xF3A3, 0xF4A4, 0xF5A5, 0xF6A6, 0xF7A7},
    {0, 1, 2, 4, 8, 16, 32, 0xFFFF}
};

void Bus(unsigned short mar, unsigned short* mdr, int read_write, int word_byte) {


    assert(word_byte == 0 || word_byte == 1);
    assert(read_write == 0 || read_write == 1);
    if (read_write == 0) {  // read = 0 
        if (word_byte == 0) {
            *mdr = memory_u.WordMem[mar >> 1];

#ifdef DEBUG
            printf(" Bus Read Word Function: ADDRESS -> %04X Memory stored -> %04X  \n", mar/2, memory_u.WordMem[mar/2]); //>> 1
#endif // DEBUG
        }
        else {
            *mdr = (unsigned char)memory_u.ByteMem[mar];
#ifdef DEBUG
            printf(" Bus Read Byte Function: ADDRESS -> %2X Memory stored -> %4x  \n", mar, memory_u.ByteMem[mar]);
#endif // DEBUG
        }
    }


    else { // write = 1 

        if (word_byte == 0) { // word = 0

            memory_u.WordMem[mar>>1] = *mdr;
#ifdef DEBUG
            printf(" Bus Write Word Function: ADDRESS -> %2X Memory stored -> %4x  \n", mar, memory_u.WordMem[mar]);
#endif // DEBUG
        }
        else {
            memory_u.ByteMem[mar] = (unsigned char)(*mdr & 0xFF);
#ifdef DEBUG
            printf(" Bus Write Byte Function: ADDRESS -> %2X Memory stored -> %4x  \n", mar, memory_u.ByteMem[mar]);
#endif // DEBUG
        }
    }


}


void Fetch_Instr() {
    //Reading an Instruction of 16 bits into Current_Insr
    unsigned short Current_Insr = 0;

    Bus(RegFile[0][7], &instr_reg, 0, 0);
    //instr_reg = Current_Insr;
    //sscanf(&Current_Insr, "%2x", &instr_reg);
#ifdef DEBUG
    printf(" Instruction to be excuted: %02x  \n", Current_Insr);
    printf("    Adress of Instruction to be excuted: %02x  \n", RegFile[0][7]>>1);
#endif // DEBUG
    RegFile[0][7] = RegFile[0][7] + 2;

}
void Control() {
    Fetch_Instr();
    int bit_array[BIT_ARRAY_SIZE];
    hexToBitArray(instr_reg, bit_array);
    
#ifdef DEBUG
    printf("Displaying Binary Array\n");
    // Print the bit array
    for (int i = 0; i < BIT_ARRAY_SIZE; i++) {
        printf("%d", bit_array[i]);
    }
    printf("\n");
    if (Hex_2_Bit(instr_reg, 12) | Hex_2_Bit(instr_reg, 11) == 1)
        printf("\n ADD INNNN\n");

#endif
    Decode(bit_array);

}
//  00000000 11010000
void printBits(unsigned short value) {

    int numBits = 16;
    for (int bit = numBits - 1; bit >= 0; --bit) {
        printf("%d", Hex_2_Bit(value, bit));
    }
    printf("\n");
}
void Decode(int *bit_array) {
    printf("Instruction: ");
#ifdef DEBUG
    printf("\n Binary being decoded : ");
    printBits(instr_reg);
    printf("BIT 15: %d\n", Hex_2_Bit(instr_reg, 15));
#endif
    /*int i = 0;
    while (i!=15)
    {
        if (Hex_2_Bit(instr_reg, i) == 1)
            printf("1");
        else if (Hex_2_Bit(instr_reg, i) == 0)
            printf("0");
        i = i + 1;
    }
    printf("\n");*/

    if (Hex_2_Bit(instr_reg,15) == 1) {
        // LDR/STR instruction
        switch (bit_array[14]) {
            case 0:
                // Code for  LDR
                break;
            case 1:
                // PUSH STR
                // Code for  STR
                break;
        }
    }
    else {
        // Branch instruction
        if (Hex_2_Bit(instr_reg, 14) == 0) {
            if (Hex_2_Bit(instr_reg, 13) == 0) {
                // Code for bL
                // R5 = &BL;
                printf("Instruction: BL\n");

            }
            else {
                if (Hex_2_Bit(instr_reg, 12) == 0) {
                    switch ((Hex_2_Bit(instr_reg, 11) << 1) | bit_array[10]) {
                    case 0:
                        // BEQ instruction
                        // Code for BEQ
                        printf("BEQ");
                        Branching();

                        break;
                    case 1:
                        // BNE instruction
                        // Code for BNE
                        printf(" BNE");
                        Branching();
                        break;
                    case 2:
                        // BC instruction
                        // Code for BC
                        printf("Instruction: BC");
                        Branching();
                        break;
                    case 3:
                        // BNC instruction
                        // Code for BNC
                        printf("Instruction: BNC");
                        Branching();
                        break;
                    }
                }
                else {
                    switch ((bit_array[11] << 1) | bit_array[10]) {
                    case 0:
                        // BN instruction
                        printf("BN\n");
                        Branching();


                        break;
                    case 1:
                        // BGE instruction
                        // Code for BGE
                        printf("BGE\n");
                        Branching();
                        break;
                    case 2:
                        // BLT instruction
                        printf("BLT");
                        Branching();
                        break;
                    case 3:
                        // BRA instruction
                        // Code for BRA
                        printf("BRA");
                        Branching();
                        break;
                    }
                }
            }
        }
        else {
            if (Hex_2_Bit(instr_reg, 12) | Hex_2_Bit(instr_reg, 11) == 00) {
                // Code for  ADD TO AND 

                if (Hex_2_Bit(instr_reg, 13) == 0) {
                    if (Hex_2_Bit(instr_reg, 10) == 0) { // ADD TILL SUBC
                        switch (Hex_2_Bit(instr_reg, 9) <<1 | Hex_2_Bit(instr_reg, 8)) {
                            case 0b00:
                                printf("ADD\n");
                                Arithmentic();
                             
                                break;
                            case 0b01:
                                printf("ADDC\n");
                                Arithmentic();

                                break;
                            case 0b10:
                                printf("SUB\n");
                                Arithmentic();
                                break;
                            case 0b11:
                                printf("SUBC\n");
                                Arithmentic();
                                break;
                        }
                    }
                    else { // DADD TO AND 
                        switch (bit_array[9] | bit_array[8]) {
                        case 0b00:
                            printf("DADD\n");
                            Arithmentic();
                            break;
                        case 0b01:
                            printf("CMP\n");
                            Arithmentic();
                            break;
                        case 0b10:
                            printf("XOR\n");
                            Arithmentic();
                            break;
                        case 0b11:
                            printf("AND\n");
                            Arithmentic();
                            break;
                        }
                    }
                }
                else {
                    // MOVL
                }
            }
            else {
               // if(Hex_2_Bit(instr_reg, 12))
            }

        }
    }

}
void Branching() {

    // shifting 1 bit to the left so that | sign |x| EncodedOffset | 0 |
    //EncodedOffset = encoded  << 1;
    //unsigned short encoded_offset = instr_reg & 0xFF80;
    //Mask all bits before then print offset 

    instr_reg = Maskbits(instr_reg);
    instr_reg = instr_reg << 1;

    printf("\nEncoded offset:  %x02\n", instr_reg);
    printf("Current PC: %x\n", RegFile[0][7]);

    if (Hex_2_Bit(instr_reg, 15) == 1)
    {
        SET_BITS(instr_reg, 7, 1);
        instr_reg = RegFile[0][7] - instr_reg;
        printf("Branch PC neg:  %x\n", RegFile[0][7] - instr_reg);
    }
    else {
        SET_BITS(instr_reg, 7, 0);
        instr_reg = RegFile[0][7] + instr_reg;
        printf("Branch PC pos:  %x\n", RegFile[0][7] + instr_reg);
    }

}

void Arithmentic() {
    printf("R/C: %d\n", Hex_2_Bit(instr_reg, 7));
    printf("W/B: %d\n", Hex_2_Bit(instr_reg, 6));

    printf("Source: %d%d%d\n", Hex_2_Bit(instr_reg, 5), Hex_2_Bit(instr_reg, 4), Hex_2_Bit(instr_reg, 3));
    unsigned short dest;
    dest = instr_reg;
    MASK_BITS(dest, 15, 3);
    dest = instr_reg &
    printf("Destination: %hx\n", dest);

}