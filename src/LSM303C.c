#include <stdint.h>

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

AxesRaw_t accel;
AxesRaw_t mag;

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
	int ret_val_acc, ret_val_mag;

	reg_I2C_tx_buffer_read[0] = MAG_WHO_AM_I;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_MAG_I2C_FLAG);
	ret_val_mag = i2c_buffer_read();

	reg_I2C_tx_buffer_read[0] = ACC_WHO_AM_I;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	ret_val_acc = i2c_buffer_read();

#endif
}

void LSM303C_GetAccelData()
{
	int8_t val_h = 0, val_l = 0;

	reg_I2C_tx_buffer_read[0] = ACC_OUT_X_L;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_l = i2c_buffer_read();
	reg_I2C_tx_buffer_read[0] = ACC_OUT_X_H;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_h = i2c_buffer_read();
	accel.xAxis = ((int16_t) ((val_h << 8) | val_l));

	reg_I2C_tx_buffer_read[0] = ACC_OUT_Y_L;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_l = i2c_buffer_read();
	reg_I2C_tx_buffer_read[0] = ACC_OUT_Y_H;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_h = i2c_buffer_read();
	accel.yAxis = ((int16_t) ((val_h << 8) | val_l));

	reg_I2C_tx_buffer_read[0] = ACC_OUT_Z_L;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_l = i2c_buffer_read();
	reg_I2C_tx_buffer_read[0] = ACC_OUT_Z_H;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_h = i2c_buffer_read();
	accel.zAxis = ((int16_t) ((val_h << 8) | val_l));
}

void LSM303C_GetMagntData()
{
	int8_t val_h = 0, val_l = 0;

	reg_I2C_tx_buffer_read[0] = MAG_OUT_X_L;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_l = i2c_buffer_read();
	reg_I2C_tx_buffer_read[0] = MAG_OUT_X_H;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_h = i2c_buffer_read();
	mag.xAxis = ((int16_t) ((val_h << 8) | val_l));

	reg_I2C_tx_buffer_read[0] = MAG_OUT_Y_L;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_l = i2c_buffer_read();
	reg_I2C_tx_buffer_read[0] = MAG_OUT_Y_H;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_h = i2c_buffer_read();
	mag.yAxis = ((int16_t) ((val_h << 8) | val_l));

	reg_I2C_tx_buffer_read[0] = MAG_OUT_Z_L;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_l = i2c_buffer_read();
	reg_I2C_tx_buffer_read[0] = MAG_OUT_Z_H;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	val_h = i2c_buffer_read();
	mag.zAxis = ((int16_t) ((val_h << 8) | val_l));
}

int LSM303C_GetAccelStatus()
{
	int status;

	reg_I2C_tx_buffer_read[0] = ACC_STATUS;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	status = i2c_buffer_read();

	return status;
}

int LSM303C_GetMagntStatus()
{
	int status;

	reg_I2C_tx_buffer_read[0] = MAG_STATUS;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	status = i2c_buffer_read();

	return status;
}
