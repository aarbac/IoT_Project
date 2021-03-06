#ifndef HEADER_I2C
#define HEADER_I2C

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
	read = 0,
	write = 1
}rd_wr;

unsigned int i2c_init_fn(void);
void i2c_read(uint8_t I2Cn_num);
void i2c_write(uint8_t I2Cn_num);
void i2c_buffer_fill(uint8_t buffer[], uint8_t rd_wr, uint8_t size, uint8_t I2C_periph_flag);
uint8_t i2c_buffer_read(void);

#endif
