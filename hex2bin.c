#include <stdio.h>
#include "emulator.h"


void hexToBitArray(unsigned int hexValue, int* bitArray) {
    for (int i = BIT_ARRAY_SIZE - 1; i >= 0; i--) {
        bitArray[i] = hexValue & 1;
        hexValue >>= 1;
    }
}