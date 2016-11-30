/**************************************************************************//**
 * @section License
 * (C) Copyright 2014 Silicon Labs, http://www.silabs.com
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"
 * for details. Before using this software for any purpose, you must agree to the
 * terms of that agreement.
 *
 *******************************************************************************
 *
 * @file
 * @brief Assignment 1- LE Timer
 * @author Aaresh Bachana
 * @version 1.4
 * @date 9/7/2016
 ******************************************************************************/

#include <stdio.h>
#include <stdbool.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_int.h"
#include "em_gpio.h"
#include "em_acmp.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_letimer.h"
#include "em_timer.h"
#include "em_adc.h"
#include "em_dma.h"
#include "em_i2c.h"
#include "em_leuart.h"
#include "em_lesense.h"
#include "dmactrl.h"

#include "sleep.h"
#include "LETimer.h"
#include "GPIO.h"
#include "SiLabs_errcodes.h"
#include "acmp.h"
#include "clock.h"
#include "LETimer.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "leuart.h"
#include "LCD.h"
#include "captouch.h"
#include "tests.h"
#include "LSM303C.h"
#include "BME280.h"
#include "calibration.h"
#include "gesture.h"

extern unsigned int test_flag;

/******************************************************************************
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
	unsigned int err_code;
	float x=0;
	/* Chip errata */
	CHIP_Init();
	BlockSleepMode(EM3);
#ifdef SELF_CALIBRATION
	if(ENERGY_MODE == EM3)
	{
		err_code = Timer_Init_fn();
		err_code = LETimer_Calibrate();
	}
#endif

	if(ENERGY_MODE == EM3)
	{
		err_code = Clock_Init(ENERGY_MODE);
		err_code = GPIO_Init(ENERGY_MODE);
#ifdef ENABLE_ON_BOARD_LIGHT_SENSOR
		err_code = ACMP_Init_fn();
#endif
		LCD_init_fn();
		//capsense_main();
		err_code = i2c_init_fn();
		err_code = adc_init_fn();
		err_code = dma_init_fn();
		//err_code = leuart_init_fn();
		//err_code = LETimer0_Init(ENERGY_MODE);
	}
	else
	{
		err_code = Clock_Init(ENERGY_MODE);
		err_code = GPIO_Init(ENERGY_MODE);
#ifdef ENABLE_ON_BOARD_LIGHT_SENSOR
		err_code = ACMP_Init_fn();
#endif
		//capsense_main();
		err_code = i2c_init_fn();
		err_code = adc_init_fn();
		err_code = dma_init_fn();
		LSM303C_init();
		err_code = LSM303C_test();
		LSM303C_GetAccelData();
		//err_code = leuart_init_fn();
		//err_code = LETimer0_Init(ENERGY_MODE);
	}

#ifdef TESTS_ON
	test_flag = 1;
	err_code = test_LCD();
	if(err_code == 0)
	{
		err_code = test_led(LOW);
	}
	if(err_code == 0)
	{
		err_code = test_led(MEDIUM);
	}
	if(err_code == 0)
	{
		err_code = test_led(HIGH);
	}
	if(err_code == 0)
	{
		err_code = test_LESENSE_captouch();
	}
	if(err_code == 0)
	{
		test_flag = 0;
	}
#endif

	//calibrate();
	writeBME280_settings();
	//x=read_temperature();
 	writeAPDS9960_settings();
	/* Infinite loop */
	while (1)
	{

		sleep();
	}
}
