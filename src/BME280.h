#ifndef HEADER_BME280
#define HEADER_BME280

#include <stdint.h>

#define  CTRL_HUMIDITY		0xF2 //Ctrl Humidity
#define  STAT				0xF3 //Status
#define  CTRL_MEAS			0xF4 //Ctrl Measure
#define  CONFIG				0xF5 //Configuration
#define  PRESSURE_MSB		0xF7 //Pressure MSB
#define  PRESSURE_LSB		0xF8 //Pressure LSB
#define  PRESSURE_XLSB		0xF9 //Pressure XLSB
#define  TEMPERATURE_MSB	0xFA //Temperature MSB
#define  TEMPERATURE_LSB	0xFB //Temperature LSB
#define  TEMPERATURE_XLSB	0xFC //Temperature XLSB
#define  HUMIDITY_MSB		0xFD //Humidity MSB
#define  HUMIDITY_LSB		0xFE //Humidity LSB

int8_t valueread(uint8_t value);
float read_pressure();
float read_altitude();
float read_temperature();
float read_humidity();
void writeBME280_settings();

#endif
