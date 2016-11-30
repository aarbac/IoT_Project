#include "BME280.h"
#include "i2c.h"
#include "main.h"
#include "calibration.h"
#include <math.h>
uint8_t I2C_rx_buffer_size = 10;
uint8_t I2C_rx_buffer[10];
int32_t t_fine;

static uint8_t reg_I2C_tx_buffer_write[2] = {0,0};
static uint8_t reg_I2C_tx_buffer_read[1] = {0};

uint8_t valueread(uint8_t value)
{
	reg_I2C_tx_buffer_read[0] = value;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, BME280_I2C_FLAG);
	uint8_t readdata=i2c_buffer_read();
	return readdata;
}

float read_pressure()
{

	// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
	// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
	int32_t adc_P = ((uint32_t)valueread(PRESSURE_MSB) << 12) | ((uint32_t)valueread(PRESSURE_LSB) << 4) | ((valueread(PRESSURE_XLSB) >> 4) & 0x0F);

	int64_t var1, var2, p_acc;
	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calibration.P6;
	var2 = var2 + ((var1 * (int64_t)calibration.P5)<<17);
	var2 = var2 + (((int64_t)calibration.P4)<<35);
	var1 = ((var1 * var1 * (int64_t)calibration.P3)>>8) + ((var1 * (int64_t)calibration.P2)<<12);
	var1 = (((((int64_t)1)<<47)+var1))*((int64_t)calibration.P1)>>33;
	if (var1 == 0)
	{
		return 0; // avoid exception caused by division by zero
	}
	p_acc = 1048576 - adc_P;
	p_acc = (((p_acc<<31) - var2)*3125)/var1;
	var1 = (((int64_t)calibration.P9) * (p_acc>>13) * (p_acc>>13)) >> 25;
	var2 = (((int64_t)calibration.P8) * p_acc) >> 19;
	p_acc = ((p_acc + var1 + var2) >> 8) + (((int64_t)calibration.P7)<<4);

	p_acc = p_acc >> 8; // /256
	return (float)p_acc;
}


float read_altitude()
{
	float heightOutput = 0;

	heightOutput = ((float)-45846.2)*(pow(((float)read_pressure()/(float)101325), 0.190263) - (float)1);
	return heightOutput;
}

float read_temperature()
{

		// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
		// t_fine carries fine temperature as global value

		//get the reading (adc_T);
	int32_t adc_T = ((uint32_t)valueread(TEMPERATURE_MSB) << 12) | ((uint32_t)valueread(TEMPERATURE_LSB) << 4) | ((valueread(TEMPERATURE_LSB) >> 4) & 0x0F);

	//By datasheet, calibrate
		int64_t var1, var2;

		var1 = ((((adc_T>>3) - ((int32_t)calibration.T1<<1))) * ((int32_t)calibration.T2)) >> 11;
		var2 = (((((adc_T>>4) - ((int32_t)calibration.T1)) * ((adc_T>>4) - ((int32_t)calibration.T1))) >> 12) * ((int32_t)calibration.T3)) >> 14;
		t_fine = var1 + var2;
		float output = (t_fine * 5 + 128) >> 8;

		output = output / 100;

		return output;

}

float read_humidity()
{
	// Returns humidity in %RH as unsigned 32 bit integer in Q22. 10 format (22 integer and 10 fractional bits).
	// Output value of “47445” represents 47445/1024 = 46. 333 %RH
	int32_t adc_H = ((uint32_t)valueread(HUMIDITY_MSB) << 8) | ((uint32_t)valueread(HUMIDITY_LSB));

	int32_t var1;
	var1 = (t_fine - ((int32_t)76800));
	var1 = (((((adc_H << 14) - (((int32_t)calibration.H4) << 20) - (((int32_t)calibration.H5) * var1)) +
	((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)calibration.H6)) >> 10) * (((var1 * ((int32_t)calibration.H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
	((int32_t)calibration.H2) + 8192) >> 14));
	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)calibration.H1)) >> 4));
	var1 = (var1 < 0 ? 0 : var1);
	var1 = (var1 > 419430400 ? 419430400 : var1);

	return (float)((var1>>12) >> 10);

}

void writeBME280_settings()
{
uint8_t data;
	reg_I2C_tx_buffer_write[0] = CTRL_MEAS;
	reg_I2C_tx_buffer_write[1] = 0x00;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, BME280_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = CONFIG;
	data = ((0x00 << 0x5) & 0xE0);
	data |= ((0x00<< 0x02) & 0x1C);
	reg_I2C_tx_buffer_write[1]=data;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, BME280_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = CTRL_HUMIDITY;
	data= (0x00 & 0x07);
	reg_I2C_tx_buffer_write[1]=data;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, BME280_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = CTRL_MEAS;
	data=(0x00 << 0x5) & 0xE0;
	data|=(0x00 << 0x02) & 0x1C;
	data|=(0x00) & 0x03;
	reg_I2C_tx_buffer_write[1] = data;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, BME280_I2C_FLAG);
}

