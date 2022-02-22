#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(){
    int readData = (PORTD & 0x0f00) >> 8; // 0000 1111 0000 0000  bits 8-11 are being read
    return readData;
}

int getbtns(){
    int readData = (PORTD & 0x0e0) >> 5; // 0000 0000 1110 0000  bits 5-7 are being read
    return readData;
}