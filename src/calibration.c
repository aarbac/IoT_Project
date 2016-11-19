#include "i2c.h"
#include "calibration.h"
#include "main.h"

static uint8_t reg_I2C_tx_buffer_write[2] = {0,0};
static uint8_t reg_I2C_tx_buffer_read[1] = {0};

int8_t calread(uint8_t value )
{
reg_I2C_tx_buffer_read[0] = value;
i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, BME280_I2C_FLAG);
int8_t readdata=i2c_buffer_read();
return readdata;
}

void calibrate()
{

	calibration.T1 = ((uint16_t)((calread(T1_MSB) << 8) + calread(T1_LSB)));
	calibration.T2 = ((int16_t)((calread(T2_MSB) << 8) + calread(T2_LSB)));
	calibration.T3 = ((int16_t)((calread(T3_MSB) << 8) + calread(T3_LSB)));

	calibration.P1 = ((uint16_t)((calread(P1_MSB) << 8) + calread(P1_LSB)));
	calibration.P2 = ((int16_t)((calread(P2_MSB) << 8) + calread(P2_LSB)));
	calibration.P3 = ((int16_t)((calread(P3_MSB) << 8) + calread(P3_LSB)));
	calibration.P4 = ((int16_t)((calread(P4_MSB) << 8) + calread(P4_LSB)));
	calibration.P5 = ((int16_t)((calread(P5_MSB) << 8) + calread(P5_LSB)));
	calibration.P6 = ((int16_t)((calread(P6_MSB) << 8) + calread(P6_LSB)));
	calibration.P7 = ((int16_t)((calread(P7_MSB) << 8) + calread(P7_LSB)));
	calibration.P8 = ((int16_t)((calread(P8_MSB) << 8) + calread(P8_LSB)));
	calibration.P9 = ((int16_t)((calread(P9_MSB) << 8) + calread(P9_LSB)));

	calibration.H1 = ((uint8_t)(calread(H1_LSB)));
	calibration.H2 = ((int16_t)((calread(H2_MSB) << 8) + calread(H2_LSB)));
	calibration.H3 = ((uint8_t)(calread(H3_LSB)));
	calibration.H4 = ((int16_t)((calread(H4_MSB) << 4) + (calread(H4_LSB) & 0x0F)));
	calibration.H5 = ((int16_t)((calread(H5_MSB) << 4) + ((calread(H4_LSB) >> 4) & 0x0F)));
	calibration.H6 = ((uint8_t)calread(H6_LSB));
}
