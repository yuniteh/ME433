#ifndef IMU_H__
#define IMU_H__

#define ADD 0x6B
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12

void init_imu();
unsigned char who();
void i2c_read_multiple(unsigned char address, unsigned char register, unsigned char * data, int length);
short temp(char * data);

#endif