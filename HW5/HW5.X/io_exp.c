#include <xc.h>
#include "io_exp.h"

void init_exp() {
    
    i2c_master_setup();
    
    // initialize io direction
    i2c_master_start();
    i2c_master_send(ADD);
    i2c_master_send(0x00);
    i2c_master_send(0xF0);
    i2c_master_stop();
 
    // set outputs to low
    i2c_master_start();
    i2c_master_send(ADD);
    i2c_master_send(0x09);
    i2c_master_send(0x00);
    i2c_master_stop();
}

void set_exp(char pin, char level) {
    unsigned char write = level << pin;
    i2c_master_start();
    i2c_master_send(ADD);
    i2c_master_send(0x09);
    i2c_master_send(write);
    i2c_master_stop();
}

char get_exp() {
    i2c_master_start();
    i2c_master_send(ADD);
    i2c_master_send(0x09);
    i2c_master_restart();
    i2c_master_send(0x41);
    char out = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();
    
    return out;
}