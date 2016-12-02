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

void LSM303C_write_reg(unsigned int reg, unsigned int data, unsigned int acc_mag_flag)
{
	if(acc_mag_flag == ACC)
	{
		reg_I2C_tx_buffer_write[0] = reg;
		reg_I2C_tx_buffer_write[1] = data;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, write, 2, LSM303C_ACC_I2C_FLAG);
	}
	else if(acc_mag_flag == MAG)
	{
		reg_I2C_tx_buffer_write[0] = reg;
		reg_I2C_tx_buffer_write[1] = data;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, write, 2, LSM303C_MAG_I2C_FLAG);
	}
}

int LSM303C_read_reg(unsigned int reg, unsigned int acc_mag_flag)
{
	int read_val;
	if(acc_mag_flag == ACC)
	{
		reg_I2C_tx_buffer_read[0] = reg;
		i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_ACC_I2C_FLAG);
	}
	else if(acc_mag_flag == MAG)
	{
		reg_I2C_tx_buffer_read[0] = reg;
		i2c_buffer_fill(reg_I2C_tx_buffer_read, read, 1, LSM303C_MAG_I2C_FLAG);
	}
	read_val = i2c_buffer_read();
	return read_val;
}

__STATIC_INLINE void Mag_set_odr(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(MAG_CTRL_REG1, MAG);

	read_val &= ~MAG_DO_80_Hz;
	val = read_val | val;

	LSM303C_write_reg(MAG_CTRL_REG1, val, MAG);
}

__STATIC_INLINE void Mag_set_fullscale(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(MAG_CTRL_REG2, MAG);

	read_val &= ~MAG_FS_16_Ga;
	val = read_val | val;

	LSM303C_write_reg(MAG_CTRL_REG2, val, MAG);
}

__STATIC_INLINE void Mag_set_blockdataupdate(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(MAG_CTRL_REG5, MAG);

	read_val &= ~MAG_BDU_ENABLE;
	val = read_val | val;

	LSM303C_write_reg(MAG_CTRL_REG5, val, MAG);
}

__STATIC_INLINE void Mag_set_XY_AxOperativeMode(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(MAG_CTRL_REG1, MAG);

	read_val &= ~MAG_OMXY_ULTRA_HIGH_PERFORMANCE;
	val = read_val | val;

	LSM303C_write_reg(MAG_CTRL_REG1, val, MAG);
}

__STATIC_INLINE void Mag_set_Z_AxOperativeMode(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(MAG_CTRL_REG4, MAG);

	read_val &= ~MAG_OMZ_ULTRA_HIGH_PERFORMANCE;
	val = read_val | val;

	LSM303C_write_reg(MAG_CTRL_REG4, val, MAG);
}

__STATIC_INLINE void Mag_set_mode(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(MAG_CTRL_REG3, MAG);

	read_val &= ~MAG_MD_POWER_DOWN_2;
	val = read_val | val;

	LSM303C_write_reg(MAG_CTRL_REG3, val, MAG);
}

__STATIC_INLINE void Acc_set_fullScale(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(ACC_CTRL_REG4, ACC);

	read_val &= ~ACC_FS_8g;
	val = read_val | val;

	LSM303C_write_reg(ACC_CTRL_REG4, val, ACC);
}

__STATIC_INLINE void Acc_set_blockdataupdate(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(ACC_CTRL_REG1, ACC);

	read_val &= ~ACC_BDU_ENABLE;
	val = read_val | val;

	LSM303C_write_reg(ACC_CTRL_REG1, val, ACC);
}

__STATIC_INLINE void Acc_set_enable_axis(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(ACC_CTRL_REG1, ACC);

	read_val &= ~0x07;
	val = read_val | val;

	LSM303C_write_reg(ACC_CTRL_REG1, val, ACC);
}

__STATIC_INLINE void Acc_set_odr(int val)
{
	int read_val;
	read_val = LSM303C_read_reg(ACC_CTRL_REG1, ACC);

	read_val &= ~ACC_ODR_MASK;
	val = read_val | val;

	LSM303C_write_reg(ACC_CTRL_REG1, val, ACC);
}

void LSM303C_init(void)
{
	Mag_set_odr(MAG_DO_40_Hz);
	Mag_set_fullscale(MAG_FS_16_Ga);
	Mag_set_blockdataupdate(MAG_BDU_ENABLE);
	Mag_set_XY_AxOperativeMode(MAG_OMXY_HIGH_PERFORMANCE);
	Mag_set_Z_AxOperativeMode(MAG_OMZ_HIGH_PERFORMANCE);
	Mag_set_mode(MAG_MD_CONTINUOUS);
	Acc_set_fullScale(ACC_FS_2g);
	Acc_set_blockdataupdate(ACC_BDU_ENABLE);
	Acc_set_enable_axis(ACC_X_ENABLE | ACC_Y_ENABLE | ACC_Z_ENABLE);
	Acc_set_odr(ACC_ODR_100_Hz);
	enable_LSM303C_interrupts();
}

int LSM303C_test(void)
{
#ifdef TESTS_ON
	int ret_val_acc, ret_val_mag, ret;

	ret_val_acc = LSM303C_read_reg(ACC_WHO_AM_I, ACC);
	ret_val_mag = LSM303C_read_reg(MAG_WHO_AM_I, MAG);

	if(ret_val_acc == 0x41 && ret_val_mag == 0x3D)
	{
		ret = 0;
	}
	else
	{
		ret = -1;
	}

	return ret;

#endif
}

void LSM303C_GetAccelData()
{
	int8_t val_h = 0, val_l = 0;

	val_l = LSM303C_read_reg(ACC_OUT_X_L, ACC);
	val_h = LSM303C_read_reg(ACC_OUT_X_H, ACC);
	accel.xAxis = ((int16_t) ((val_h << 8) | val_l));

	val_l = LSM303C_read_reg(ACC_OUT_Y_L, ACC);
	val_h = LSM303C_read_reg(ACC_OUT_Y_H, ACC);
	accel.yAxis = ((int16_t) ((val_h << 8) | val_l));

	val_l = LSM303C_read_reg(ACC_OUT_Z_L, ACC);
	val_h = LSM303C_read_reg(ACC_OUT_Z_H, ACC);
	accel.zAxis = ((int16_t) ((val_h << 8) | val_l));
}

void LSM303C_GetMagntData()
{
	int8_t val_h = 0, val_l = 0;

	val_l = LSM303C_read_reg(MAG_OUT_X_L, MAG);
	val_h = LSM303C_read_reg(MAG_OUT_X_H, MAG);
	mag.xAxis = ((int16_t) ((val_h << 8) | val_l));

	val_l = LSM303C_read_reg(MAG_OUT_Y_L, MAG);
	val_h = LSM303C_read_reg(MAG_OUT_Y_H, MAG);
	mag.yAxis = ((int16_t) ((val_h << 8) | val_l));

	val_l = LSM303C_read_reg(MAG_OUT_Z_L, MAG);
	val_h = LSM303C_read_reg(MAG_OUT_Z_H, MAG);
	mag.zAxis = ((int16_t) ((val_h << 8) | val_l));
}

int LSM303C_GetAccelStatus()
{
	int status;
	status = LSM303C_read_reg(ACC_STATUS, ACC);
	return status;
}

int LSM303C_GetMagntStatus()
{
	int status;
	status = LSM303C_read_reg(MAG_STATUS, MAG);
	return status;
}

void enable_LSM303C_interrupts(void)
{
	uint8_t val = 0xEF;
	LSM303C_write_reg(MAG_INT_CFG, val, MAG);
	//The interrupt will be cleared when MAG_INT_SRC is read

	val = 0x00;
	LSM303C_write_reg(MAG_INT_THS_L, val, MAG);
	val = 0x01;
	LSM303C_write_reg(MAG_INT_THS_H, val, MAG);
}


