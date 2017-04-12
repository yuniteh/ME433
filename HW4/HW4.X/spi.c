#include "spi.h"
#include <xc.h>

#define CS LATAbits.LATA0

void init_spi() {
    // assign pin functions
    RPA1Rbits.RPA1R = 0b0011; //SDO1
    SDI1Rbits.SDI1R = 0b0100; //SDI1

    TRISAbits.TRISA0 = 0;
    CS = 1;

    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 0x1; // baud rate to 10 MHz [SPI4BRG = (80000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 1
}

unsigned char spi1_io(unsigned char o) {
    SPI1BUF = o;
    while (!SPI1STATbits.SPIRBF) { // wait to receive the byte
        ;
    }
    return SPI1BUF;
}

void set_voltage(unsigned char channel, unsigned char voltage) {
    CS = 0;
    unsigned char out_1 = voltage >> 4;
    unsigned char out_2 = voltage << 4;
    if(channel == 1) {
        out_1 = out_1 + 0x80;
    }
    out_1 = out_1 + 0x30;
    spi1_io(out_1);
    spi1_io(out_2);
    CS = 1;
}