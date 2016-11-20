#ifndef HEADER_LSM303C
#define HEADER_LSM303C

#include <stdint.h>

#define SENSITIVITY_ACC 0.06103515625
#define SENSITIVITY_MAG 0.00048828125

typedef enum
{
	MAG_WHO_AM_I   	= 0x0F,
	MAG_CTRL_REG1  	= 0x20,
	MAG_CTRL_REG2  	= 0x21,
	MAG_CTRL_REG3  	= 0x22,
	MAG_CTRL_REG4 	= 0x23,
	MAG_CTRL_REG5	= 0x24,
	MAG_STATUS		= 0x27,
	MAG_OUT_X_L     = 0x28,
	MAG_OUT_X_H     = 0x29,
	MAG_OUT_Y_L     = 0x2A,
	MAG_OUT_Y_H     = 0x2B,
	MAG_OUT_Z_L     = 0x2C,
	MAG_OUT_Z_H     = 0x2D,
	MAG_TEMP_OUT_L 	= 0x2E,
	MAG_TEMP_OUT_H 	= 0x2F,
	MAG_INT_CFG    	= 0x30,
	MAG_INT_SRC    	= 0x31,
	MAG_INT_THS_L  	= 0x32,
	MAG_INT_THS_H  	= 0x33
} MAG_REG_t;

/////////////////////////////////////
// LSM303C Accelerometer Registers //
/////////////////////////////////////
typedef enum
{
	ACC_WHO_AM_I     = 0x0F,
	ACC_ACT_TSH      = 0x1E,
	ACC_ACT_DUR      = 0x1F,
	ACC_CTRL1        = 0x20,
	ACC_CTRL2        = 0x21,
	ACC_CTRL3        = 0x22,
	ACC_CTRL4        = 0x23,
	ACC_CTRL5        = 0x24,
	ACC_CTRL6        = 0x25,
	ACC_CTRL7        = 0x26,
	ACC_STATUS       = 0x27,
	ACC_OUT_X_L      = 0x28,
	ACC_OUT_X_H      = 0x29,
	ACC_OUT_Y_L      = 0x2A,
	ACC_OUT_Y_H      = 0x2B,
	ACC_OUT_Z_L      = 0x2C,
	ACC_OUT_Z_H      = 0x2D,
	ACC_FIFO_CTRL    = 0x2E,
	ACC_FIFO_SRC     = 0x2F,
	ACC_IG_CFG1      = 0x30,
	ACC_IG_SRC1      = 0x31,
	ACC_IG_THS_X1    = 0x32,
	ACC_IG_THS_Y1    = 0x33,
	ACC_IG_THS_Z1    = 0x34,
	ACC_IG_DUR1      = 0x35,
	ACC_IG_CFG2      = 0x36,
	ACC_IG_SRC2      = 0x37,
	ACC_IG_THS2      = 0x38,
	ACC_IG_DUR2      = 0x39,
	ACC_XL_REFERENCE = 0x3A,
	ACC_XH_REFERENCE = 0x3B,
	ACC_YL_REFERENCE = 0x3C,
	ACC_YH_REFERENCE = 0x3D,
	ACC_ZL_REFERENCE = 0x3E,
	ACC_ZH_REFERENCE = 0x3F
} ACC_REG_t;

typedef enum
{
	MAG_TEMP_EN_DISABLE = 0x00,
	MAG_TEMP_EN_ENABLE  = 0x80
} MAG_TEMP_EN_t;

typedef enum
{
	MAG_XYZDA_NO  = 0x00,
	MAG_XYZDA_YES = 0x08
} MAG_XYZDA_t;

typedef enum
{
	ACC_I2C_ADDR = 0x1D,
	MAG_I2C_ADDR = 0x1E
} I2C_ADDR_t;

typedef struct
{
	int16_t xAxis;
	int16_t yAxis;
	int16_t zAxis;
} AxesRaw_t;

typedef enum
{
	MODE_SPI,
	MODE_I2C,
} InterfaceMode_t;

typedef enum
{
	MAG,
	ACC
} CHIP_t;

typedef enum
{
	xAxis,
	yAxis,
	zAxis
} AXIS_t;

typedef enum
{
	MAG_DO_0_625_Hz = 0x00,
	MAG_DO_1_25_Hz  = 0x04,
	MAG_DO_2_5_Hz   = 0x08,
	MAG_DO_5_Hz     = 0x0C,
	MAG_DO_10_Hz    = 0x10,
	MAG_DO_20_Hz    = 0x14,
	MAG_DO_40_Hz    = 0x18,
	MAG_DO_80_Hz    = 0x1C
} MAG_DO_t;

void LSM303C_init(void);
void LSM303C_write(unsigned int reg, unsigned int data);
int LSM303C_read(unsigned int reg);
void LSM303C_enable(void);
void LSM303C_disable(void);

#endif
