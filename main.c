/*TO RUN PROGRAM ON Visual Studio go to debug properties > c/c++ > preprocessor > _CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "emulator.h"
#include <stdbool.h>

#define MEM_SIZE 0x10000
#define MAXBufSize 256
//#define DEBUG
union Memory memory_u;
unsigned short instr_reg;



unsigned char testing_input[MAXBufSize]; // Define testing_input variable

void CheckCheckSum(int length) {
    unsigned char sum = 0;
    unsigned int value1 =0 ;
    int values_assigned;
    for (int i = 2; i < (length * 2) + 4; i = i + 2) {
        sum += value1;
        values_assigned = sscanf(&testing_input[i], "%2x", &value1);
        // value1 = 0;
    }

    /*if (sum == 255){
         printf("Check Sum Valid\n");
     }*/
    if (sum != 255) {
        printf("Error: Check Sum Error detected file corrupted\n");
        printf("CHECKSUM %02x\n ", sum);
        printf("Corrupted S-record: %s", testing_input);
        //exit(0);
    }
}

void ReadFile(FILE* in_file) {
    unsigned char CheckSum = 0;



    while (fgets(testing_input, MAXBufSize, in_file) != NULL) {
        if (testing_input[0] != 'S') {
            printf("ERROR: %s IS Not a valid S-record \n", testing_input);
        }
        else if (testing_input[1] != '0' && testing_input[1] != '1' && testing_input[1] != '9') {
            printf("ERROR: %s IS Not a supported S-record only S0, S1, S9\n", testing_input);
        }
        unsigned int address_lo;
        unsigned int address_hi;
        sscanf(&testing_input[4], "%2x%2x", &address_lo, &address_hi);
        unsigned short address = (address_lo << 8) | address_hi;
        int length;

        sscanf(&testing_input[2], "%2x", &length);

        int data_length = (length * 2);
        //CheckCheckSum(length);
#ifdef DEBUG
        printf("Payload Length: %i bytes \n", data_length);
#endif

        if (testing_input[1] == '0') {
            // Display File Nom
            char filename[100];
            printf("FILE NAME: ");
            int test = 0;
            for (int i = 0; i < data_length; i = i + 2) {
                sscanf(&testing_input[i], "%2x", &filename[i]);
                printf("%c", filename[i]);
                CheckSum = filename[i] + CheckSum;
            }
            printf("\n");



        }

        else if (testing_input[1] == '1') {
            /*The program must extract the address of the first byte in the record. Subsequent
            data bytes are stored in contiguous locations. If there is already a value in a memory location, it must
            be overwritten.
            */

            // Overwrite memory locations with data bytes
            unsigned short byte ;
            unsigned short byte1 ;
            //   MemWrite(address + i / 2, (char)byte);
            int loop_end = data_length + 2;
            int i_adr = address;
            unsigned short input;
            for (int i = 8; i < loop_end; i += 2) {
                // Writing to address one byte at a time 
                sscanf(&testing_input[i],  "%2hx", &byte);

                Bus(i_adr, &byte, 1, 1);
                CheckSum = (char)byte + CheckSum;
                i_adr+=1;
            }


        }
        else if (testing_input[1] == '9') {

            RegFile[0][7] = address ;
#ifdef DEBUG
            printf("Adress of Instruction Register%2X\n", RegFile[0][7]);
#endif




        }

        int number_of_digits = (length * 2) + 4; // SumOfDigits = (length*2) + header (2) + Count (2)
        // CheckSum 
        char checksum[2];
        //printf("checksum size %i", sizeof(checksum));
        sscanf(&testing_input[number_of_digits - 2], "%c", &checksum[0]);
        sscanf(&testing_input[number_of_digits - 1], "%c", &checksum[1]);
        //printf("CHECKSUM: %c%c\n", checksum[0], checksum[1]);

        CheckSum = ~(CheckSum);
        printf("CHECKSUM %02x\n", CheckSum);
    }

}


int main(int argc, char* argv[]) {
    const char* file_name = "file_name";
    FILE* in_file;
    // FIX ARV<-1
    //error_flag = fopen_s(&in_file, argv[1], "r");
    //fopen_s(&in_file, "ArrayInit.xme", "r");
    fopen_s(&in_file, "branching.xme", "r");
    //fopen_s(&in_file, "BitCount1.xme", "r");
    //fopen_s(&in_file, "test2.xme", "r");
    if (in_file == NULL) {
        printf("Error: No Input file");

    }
    bool debug = true;

    ReadFile(in_file);
    char* primitive[] = { "C", "c", "E", "e", "PC", "pc"};
    char input[2];
    

    while(debug == true)
    {   
        printf("Enter: \n");
        printf("-> C to continue to next instruction \n");
        printf("-> E to END to Program \n");
        printf("-> PC to change the program counter \n");
        scanf("%s", input);
        
        if (!strcmp(primitive[0], input) || !strcmp(primitive[1], input)) {
            printf("Continuing....\n");
            Control();

        }
        else if (!strcmp(primitive[2], input) || !strcmp(primitive[2], input)) {
            printf("Halting program\n");
            debug = false;
        }
        else if (!strcmp(primitive[4], input) || !strcmp(primitive[5], input)) {
            printf("Enter value for PC (In Hex Form): ");
            scanf("%02x", input);
            RegFile[0][7] = input;
            Control();
        }
        else {
            printf("Humman Error: That is not an option\n");
        }
       
        
    }


   // printf("Binary: %s\n", binaryString);
    fclose(in_file);
    return 0;

}


