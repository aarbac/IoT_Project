#include "LSM303C.h"
#include "main.h"
#include "GPIO.h"
#include "i2c.h"

#include "em_int.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_i2c.h"

static uint8_t reg_I2C_tx_buffer_write[2] = {0,0};
static uint8_t reg_I2C_tx_buffer_read[1] = {0};

void LSM303C_init(void)
{

}

void LSM303C_write(unsigned int reg, unsigned int data)
{

}

int LSM303C_read(unsigned int reg)
{
	int read_val;

	return read_val;
}

void LSM303C_enable(void)
{

}

void LSM303C_disable(void)
{

}

void LSM303C_test(void)
{
#ifdef TESTS_ON

	reg_I2C_tx_buffer_read[0] = MAG_WHO_AM_I;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1);

	reg_I2C_tx_buffer_read[0] = ACC_WHO_AM_I;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1);

#endif
}
