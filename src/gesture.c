#include "i2c.h"
#include "gesture.h"
#include "main.h"

static uint8_t reg_I2C_tx_buffer_write[2] = {0,0};
static uint8_t reg_I2C_tx_buffer_read[1] = {0};

int8_t valueread(uint8_t value)
{
reg_I2C_tx_buffer_read[0] = value;
i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, APDS9960_I2C_FLAG);
int8_t readdata=i2c_buffer_read();
return readdata;
}

proxdata=valueread(REG_PDATA); //refer page 12,17 and 33 to clear interrupts of gesture and proximity

void writeAPDS9960_settings()
{
	reg_I2C_tx_buffer_write[0] = REG_ENABLE;
	reg_I2C_tx_buffer_write[1] = 0x00;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_WTIME;
	reg_I2C_tx_buffer_write[1] = WTIME;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_PPULSE;
	reg_I2C_tx_buffer_write[1] = PROX_PPULSE;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_POFFSET_UR;
	reg_I2C_tx_buffer_write[1] = POFFSET_UR;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_POFFSET_DL;
	reg_I2C_tx_buffer_write[1] = POFFSET_DL;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_CONFIG1;
	reg_I2C_tx_buffer_write[1] = CONFIG1;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	uint8_t drive=0;
	uint8_t val;
	drive &= 0b00000011;
	drive = drive << 6;
	val &= 0b00111111;
	val |= drive;

	reg_I2C_tx_buffer_write[0] = REG_CONTROL;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	val=0;
	drive=PGAIN;
	drive &= 0b00000011;
	drive = drive << 6;
	val &= 0b00111111;
	val |= drive;

	reg_I2C_tx_buffer_write[0] = REG_CONTROL;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	val=0;
	drive=AGAIN;
    drive &= 0b00000011;
    val &= 0b11111100;
    val |= drive;

	reg_I2C_tx_buffer_write[0] = REG_CONTROL;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_PILT;
	reg_I2C_tx_buffer_write[1] = PILT;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);


	reg_I2C_tx_buffer_write[0] = REG_PIHT;
	reg_I2C_tx_buffer_write[1] = PIHT;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_PERS;
	reg_I2C_tx_buffer_write[1] = PERS;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	uint8_t boost;
	val=0;
    boost &= 0b00000011;
    boost = boost << 4;
    val &= 0b11001111;
    val |= boost;
	reg_I2C_tx_buffer_write[0] = REG_CONFIG2;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_CONFIG3;
	reg_I2C_tx_buffer_write[1] = CONFIG3;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);


	reg_I2C_tx_buffer_write[0] = REG_GPENTH;
	reg_I2C_tx_buffer_write[1] = GPENTH;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_GEXTH;
	reg_I2C_tx_buffer_write[1] = GEXTH;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_GCONF1;
	reg_I2C_tx_buffer_write[1] = GCONF1;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	val=0;
	uint8_t gain;
	gain=GGAIN;
    gain &= 0b00000011;
    gain = gain << 5;
    val &= 0b10011111;
    val |= gain;

	reg_I2C_tx_buffer_write[0] = REG_GCONF2;
	reg_I2C_tx_buffer_write[1] =val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	val=0;
	drive=GLDRIVE;
    drive &= 0b00000011;
    drive = drive << 3;
    val &= 0b11100111;
    val |= drive;
	reg_I2C_tx_buffer_write[0] = REG_GCONF2;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	val=0;
	uint8_t time;
	drive=GWTIME;
	time &= 0b00000111;
	val &= 0b11111000;
	val |= time;
	reg_I2C_tx_buffer_write[0] = REG_GCONF2;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_GOFFSET_U;
	reg_I2C_tx_buffer_write[1] = GOFFSET;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_GOFFSET_D;
	reg_I2C_tx_buffer_write[1] = GOFFSET;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);


	reg_I2C_tx_buffer_write[0] = REG_GOFFSET_L;
	reg_I2C_tx_buffer_write[1] = GOFFSET;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_GOFFSET_R;
	reg_I2C_tx_buffer_write[1] = GOFFSET;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_GPULSE;
	reg_I2C_tx_buffer_write[1] = GPULSE;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_GCONF3;
	reg_I2C_tx_buffer_write[1] = GCONF3;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	uint8_t mode;
	val=0;
	mode=GIEN;
    mode &= 0b00000001;
    val &= 0b11111110;
    val |= mode;
	reg_I2C_tx_buffer_write[0] = REG_GCONF4;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_ENABLE;
	reg_I2C_tx_buffer_write[1] = 0x6D;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);
}
