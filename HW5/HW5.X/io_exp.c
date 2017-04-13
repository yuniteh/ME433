#include <xc.h>

void init_exp() {
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB3 = 0;
    
    i2c_master_setup();
    
    // initialize io direction
    i2c_master_start();
    i2c_master_send(0x40);
    i2c_master_send(0x00);
    i2c_master_send(0x0F);
    
    // set outputs to low
    i2c_master_restart();
    i2c_master_send(0x40);
    i2c_master_send(0x09);
    i2c_master_send(0x00);
    i2c_master_stop();
}

void set_exp(char pin, char level) {
    unsigned char write = level << pin;
    i2c_master_start();
    i2c_master_send(0x40);
    i2c_master_send(0x09);
    i2c_master_send(write);
    i2c_master_stop();
}

char get_exp() {

}