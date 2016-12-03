#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "i2c.h"
#include "gesture.h"
#include "main.h"
#include "em_gpio.h"
#include "LCD.h"
static uint8_t reg_I2C_tx_buffer_write[2] = {0,0};
static uint8_t reg_I2C_tx_buffer_read[1] = {0};

struct gesture_data {
    uint8_t udata[32];
    uint8_t ddata[32];
    uint8_t ldata[32];
    uint8_t rdata[32];
    uint8_t index;
    uint8_t total;
    uint8_t inthresh;
    uint8_t outthresh;
} gesture_data;

void GPIO_EVEN_IRQHandler(void)
{
	uint8_t val=readgesture();
}

void gpio_interrupt_enable()
{
	GPIO_IntConfig(gpioPortC, 6, false, true, true);
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

uint8_t valueread1(uint8_t value)
{
	reg_I2C_tx_buffer_read[0] = value;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, APDS9960_I2C_FLAG);
	uint8_t readdata=i2c_buffer_read();
	return readdata;
}

//proxdata=valueread1(REG_PDATA); //refer page 12,17 and 33 to clear interrupts of gesture and proximity

void writeAPDS9960_settings()
{

	reg_I2C_tx_buffer_write[0] = REG_ENABLE;
	reg_I2C_tx_buffer_write[1] = 0x00;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_ATIME;
	reg_I2C_tx_buffer_write[1] = ATIME;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_WTIME; //Gest settings
	reg_I2C_tx_buffer_write[1] = WTIME;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_PPULSE; //Gest settings
	reg_I2C_tx_buffer_write[1] = GESTURE_PPULSE;
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

	uint8_t drive;
	uint8_t val;
	drive=0;
	drive &= 0b00000011;
	drive = drive << 6;
	val=valueread1(REG_CONTROL);
	val &= 0b00111111;
	val |= drive;

	reg_I2C_tx_buffer_write[0] = REG_CONTROL;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);


	drive=PGAIN;
	drive &= 0b00000011;
	drive = drive << 6;
	val=valueread1(REG_CONTROL);
	val &= 0b00111111;
	val |= drive;

	reg_I2C_tx_buffer_write[0] = REG_CONTROL;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);


	drive=AGAIN;
    drive &= 0b00000011;
    val=valueread1(REG_CONTROL);
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

    uint8_t val_low;
    uint8_t val_high;

    /* Break 16-bit threshold into 2 8-bit values */
    val_low = AILT & 0x00FF;
    val_high = (AILT & 0xFF00) >> 8;

	reg_I2C_tx_buffer_write[0] = REG_AILTL;
	reg_I2C_tx_buffer_write[1] = val_low;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_AILTH;
	reg_I2C_tx_buffer_write[1] = val_high;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

    /* Break 16-bit threshold into 2 8-bit values */
    val_low = AIHT & 0x00FF;
    val_high = (AIHT & 0xFF00) >> 8;

	reg_I2C_tx_buffer_write[0] = REG_AIHTL;
	reg_I2C_tx_buffer_write[1] = val_low;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_AIHTH;
	reg_I2C_tx_buffer_write[1] = val_high;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_PERS;
	reg_I2C_tx_buffer_write[1] = PERS;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = REG_CONFIG2;
	reg_I2C_tx_buffer_write[1] = CONFIG2;
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

	uint8_t gain;
	gain=GGAIN;
    gain &= 0b00000011;
    gain = gain << 5;
    val=valueread1(REG_GCONF2);
    val &= 0b10011111;
    val |= gain;

	reg_I2C_tx_buffer_write[0] = REG_GCONF2;
	reg_I2C_tx_buffer_write[1] =val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	drive=GLDRIVE;
    drive &= 0b00000011;
    drive = drive << 3;
    val=valueread1(REG_GCONF2);
    val &= 0b11100111;
    val |= drive;
	reg_I2C_tx_buffer_write[0] = REG_GCONF2;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	drive=GWTIME;
	drive &= 0b00000111;
	val=valueread1(REG_GCONF2);
	val &= 0b11111000;
	val |= drive;
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


	drive=GIEN;
    drive &= 0b00000001;
    drive = drive << 1;
    val=valueread1(REG_GCONF4);
    val &= 0b11111101;
    val |= drive;
	reg_I2C_tx_buffer_write[0] = REG_GCONF4;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);
	//defaults till here

	//gesture specific now
	val=0;
	drive=3;
    drive &= 0b00000011;
    drive = drive << 4;
    val=valueread1(REG_CONFIG2);
    val &= 0b11001111;
    val |= drive;
	reg_I2C_tx_buffer_write[0] = REG_CONFIG2;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	drive=1;
	drive &= 0b00000001;
	drive = drive << 1;
	val=valueread1(REG_GCONF4);
    val &= 0b11111101;
    val |= drive;
	reg_I2C_tx_buffer_write[0] = REG_GCONF4;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	drive=1;
	drive &= 0b00000001;
	val=valueread1(REG_GCONF4);
    val &= 0b11111110;
    val |= drive;
	reg_I2C_tx_buffer_write[0] = REG_GCONF4;
	reg_I2C_tx_buffer_write[1] = val;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);

	//power

	reg_I2C_tx_buffer_write[0] = REG_ENABLE;
	reg_I2C_tx_buffer_write[1] = 0x4D;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, APDS9960_I2C_FLAG);
}

void decodegesture()
{
    /* Determine swipe direction */
    if( (gesture_ud_c == -1) && (gesture_lr_c == 0) )
    {
        gesture_motion = UP;
        LCD_write_string("UP");
    }
    else if( (gesture_ud_c == 1) && (gesture_lr_c == 0) )
    {
        gesture_motion = DOWN;
        LCD_write_string("DOWN");
    }
    else if( (gesture_ud_c == 0) && (gesture_lr_c == 1) )
    {
        gesture_motion = RIGHT;
        LCD_write_string("RIGHT");
    }
    else if( (gesture_ud_c == 0) && (gesture_lr_c == -1) )
    {
        gesture_motion = LEFT;
        LCD_write_string("LEFT");
    }
    else if( (gesture_ud_c == -1) && (gesture_lr_c == 1) )
    {
        if( abs(gesture_ud_d) > abs(gesture_lr_d) )
        {
            gesture_motion = UP;
            LCD_write_string("UP");
        } else
        {
            gesture_motion = RIGHT;
            LCD_write_string("RIGHT");
        }
    }
    else if( (gesture_ud_c == 1) && (gesture_lr_c == -1) )
    {
        if( abs(gesture_ud_d) > abs(gesture_lr_d) )
        {
            gesture_motion = DOWN;
            LCD_write_string("DOWN");
        }
        else {
            gesture_motion = LEFT;
            LCD_write_string("LEFT");
        }
    }
    else if( (gesture_ud_c == -1) && (gesture_lr_c == -1) )
    {
        if( abs(gesture_ud_d) > abs(gesture_lr_d) )
        {
            gesture_motion = UP;
            LCD_write_string("UP");
        }
        else
        {
            gesture_motion = LEFT;
            LCD_write_string("LEFT");
        }
    }
    else if( (gesture_ud_c == 1) && (gesture_lr_c == 1) )
    {
        if( abs(gesture_ud_d) > abs(gesture_lr_d) )
        {
            gesture_motion = DOWN;
            LCD_write_string("DOWN");
        }
        else
        {
            gesture_motion = RIGHT;
            LCD_write_string("RIGHT");
        }
    }
}


bool processgesture()
{
    uint8_t ufirst = 0;
    uint8_t dfirst = 0;
    uint8_t lfirst = 0;
    uint8_t rfirst = 0;
    uint8_t ulast = 0;
    uint8_t dlast = 0;
    uint8_t llast = 0;
    uint8_t rlast = 0;
    int udratio_first;
    int lrratio_first;
    int udratio_last;
    int lrratio_last;
    int ud_d;
    int lr_d;
    int i;

    if( gesture_data.total <= 4 )
    {
            return false;
    }
    if( (gesture_data.total<= 32) && (gesture_data.total> 0) )
    {
        /* Find the first value in U/D/L/R above the threshold */
        for(i=0;i<gesture_data.total;i++ )
        {
            if( (gesture_data.udata[i] > GESTURE_THRESHOLD) &&
                (gesture_data.ddata[i] > GESTURE_THRESHOLD) &&
                (gesture_data.ldata[i] > GESTURE_THRESHOLD) &&
                (gesture_data.rdata[i] > GESTURE_THRESHOLD) )
            {
                ufirst = gesture_data.udata[i];
                dfirst = gesture_data.ddata[i];
                lfirst = gesture_data.ldata[i];
                rfirst = gesture_data.rdata[i];
                break;
            }
        }

        if( (ufirst == 0) || (dfirst == 0) || (lfirst == 0) || (rfirst == 0) )
        {
                    return false;
        }

        /* Find the last value in U/D/L/R above the threshold */
        for( i = gesture_data.total - 1; i >= 0; i-- )
        {

            if( (gesture_data.udata[i] > GESTURE_THRESHOLD) &&
                (gesture_data.ddata[i] > GESTURE_THRESHOLD) &&
                (gesture_data.ldata[i] > GESTURE_THRESHOLD) &&
                (gesture_data.rdata[i] > GESTURE_THRESHOLD) )
            {
                ulast = gesture_data.udata[i];
                dlast = gesture_data.ddata[i];
                llast = gesture_data.ldata[i];
                rlast = gesture_data.rdata[i];
                break;
            }
        }
    }
    /* Calculate the first vs. last ratio of up/down and left/right */
    udratio_first = (int)(((ufirst - dfirst) * 100) / (ufirst + dfirst));
    lrratio_first = (int)((lfirst - rfirst) * 100) / (lfirst + rfirst);
    udratio_last = (int)((ulast - dlast) * 100) / (ulast + dlast);
    lrratio_last = (int)((llast - rlast) * 100) / (llast + rlast);

    /* Determine the difference between the first and last ratios */
    ud_d = (int)(udratio_last - udratio_first);
    lr_d = (int)(lrratio_last - lrratio_first);

    /* Accumulate the UD and LR delta values */
    gesture_ud_d += ud_d;
    gesture_lr_d += lr_d;

    /* Determine U/D gesture */
    if( gesture_ud_d >= GESTURE_SENSITIVITY )
    {
        gesture_ud_c = 1;
    }
    else if( gesture_ud_d <= -GESTURE_SENSITIVITY )
    {
        gesture_ud_c = -1;
    }
    else
    {
        gesture_ud_c = 0;
    }

    /* Determine L/R gesture */
    if( gesture_lr_d >= GESTURE_SENSITIVITY )
    {
        gesture_lr_c = 1;
    }
    else if( gesture_lr_d <= -GESTURE_SENSITIVITY )
    {
        gesture_lr_c= -1;
    }
    else
    {
        gesture_lr_c = 0;
    }

}

void delay(uint8_t x)
{
	   int c = 0;
	   for ( c = 0 ; c < x*x ; c++ )
	   {;}
}

uint8_t readgesture()
{
    uint8_t fifosize = 0;
    uint8_t fifodata[128];
    uint8_t gstatus;
    static uint8_t q = 1;
    int i;

    while(1)
    {
    	delay(FIFO_TIME);
    	gstatus=valueread1(REG_GSTATUS);
    	if( ( gstatus& GVALID) == GVALID )
    	{
    		fifosize=valueread1(REG_GFLVL);
			for(i = 0; i < fifosize*4; i+=4)
			{
				fifodata[i]=valueread1(REG_GFIFO_U);
				fifodata[i+1]=valueread1(REG_GFIFO_D);
				fifodata[i+2]=valueread1(REG_GFIFO_L);
				fifodata[i+3]=valueread1(REG_GFIFO_R);
			}

			for(i=0; i<fifosize*4; i+=4)
			{
				gesture_data.udata[gesture_data.index] = fifodata[i + 0];
				gesture_data.ddata[gesture_data.index] = fifodata[i + 1];
				gesture_data.ldata[gesture_data.index] = fifodata[i + 2];
				gesture_data.rdata[gesture_data.index] = fifodata[i + 3];
				gesture_data.index++;
				gesture_data.total++;
			}

			processgesture();
			decodegesture();
			gesture_data.index = 0;
			gesture_data.total = 0;

    	}
    	else
    	{
			/* Filter and p rocess gesture data. Decode near/far state */
			delay(FIFO_TIME);
			decodegesture();
			/* Reset data */
			gesture_data.index = 0;
			gesture_data.total = 0;
			gesture_ud_d = 0;
			gesture_lr_d = 0;
			gesture_ud_c = 0;
			gesture_lr_c = 0;
			gesture_motion = NONE;
			return 1;
    	}
    }
}
