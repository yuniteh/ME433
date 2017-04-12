#ifndef SPI_H
#define	SPI_H

void init_spi();
unsigned char spi1_io(unsigned char write);
void set_voltage(unsigned char channel, unsigned char voltage);

#endif 