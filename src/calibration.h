#ifndef HEADER_CALIBRATION
#define HEADER_CALIBRATION

#define  T1_LSB		0x88
#define  T1_MSB		0x89
#define  T2_LSB		0x8A
#define  T2_MSB		0x8B
#define  T3_LSB		0x8C
#define  T3_MSB		0x8D
#define  P1_LSB		0x8E
#define  P1_MSB		0x8F
#define  P2_LSB		0x90
#define  P2_MSB		0x91
#define  P3_LSB		0x92
#define  P3_MSB		0x93
#define  P4_LSB		0x94
#define  P4_MSB		0x95
#define  P5_LSB		0x96
#define  P5_MSB		0x97
#define  P6_LSB		0x98
#define  P6_MSB		0x99
#define  P7_LSB		0x9A
#define  P7_MSB		0x9B
#define  P8_LSB		0x9C
#define  P8_MSB		0x9D
#define  P9_LSB		0x9E
#define  P9_MSB		0x9F
#define  H1_LSB		0xA1
#define  H2_LSB		0xE1
#define  H2_MSB		0xE2
#define  H3_LSB		0xE3
#define  H4_MSB		0xE4
#define  H4_LSB		0xE5
#define  H5_MSB		0xE6
#define  H6_LSB		0xE7

struct calibratesensor
{
	uint16_t T1;
	int16_t T2;
	int16_t T3;

	uint16_t P1;
	int16_t P2;
	int16_t P3;
	int16_t P4;
	int16_t P5;
	int16_t P6;
	int16_t P7;
	int16_t P8;
	int16_t P9;

	uint8_t H1;
	int16_t H2;
	uint8_t H3;
	int16_t H4;
	int16_t H5;
	uint8_t H6;
}calibration;

int8_t calread(uint8_t value);
void calibrate();

#endif
