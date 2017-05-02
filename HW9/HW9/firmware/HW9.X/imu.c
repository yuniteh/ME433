#include <xc.h>
#include "imu.h"

void init_imu() {
    i2c_master_setup();

    i2c_master_start();
    i2c_master_send(ADD << 1 | 0);
    i2c_master_send(CTRL1_XL);
    i2c_master_send(0x82);
    i2c_master_stop();

    i2c_master_start();
    i2c_master_send(ADD << 1 | 0);
    i2c_master_send(CTRL2_G);
    i2c_master_send(0x88);
    i2c_master_stop();

    i2c_master_start();
    i2c_master_send(ADD << 1 | 0);
    i2c_master_send(CTRL3_C);
    i2c_master_send(0x04);
    i2c_master_stop();
}

unsigned char who() {
    i2c_master_start();
    i2c_master_send(ADD << 1 | 0);
    i2c_master_send(0x0F);
    i2c_master_restart();
    i2c_master_send(ADD << 1 | 1);
    char out = i2c_master_recv();
    i2c_master_ack(1);
    i2c_master_stop();

    return out;
}

void i2c_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length) {
    i2c_master_start();
    i2c_master_send(address << 1 | 0);
    i2c_master_send(reg);
    i2c_master_restart();
    i2c_master_send(address << 1 | 1);
    int i;
    for (i = 0; i < length; i++) {
        data[i] = i2c_master_recv();
        if (i < length - 1) {
            i2c_master_ack(0);
        } else {
            i2c_master_ack(1);
        }
    }
    i2c_master_stop();
}
