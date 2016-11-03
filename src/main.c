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
#include "tsl2651.h"
#include "leuart.h"
#include "LCD.h"

/******************************************************************************
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
	unsigned int err_code;
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
		//err_code = i2c_init_fn();
		err_code = adc_init_fn();
		err_code = dma_init_fn();
		//err_code = leuart_init_fn();
		err_code = LETimer0_Init(ENERGY_MODE);
	}
	else
	{
		err_code = Clock_Init(ENERGY_MODE);
		err_code = GPIO_Init(ENERGY_MODE);
#ifdef ENABLE_ON_BOARD_LIGHT_SENSOR
		err_code = ACMP_Init_fn();
#endif
		LCD_write_number(32);
		//err_code = i2c_init_fn();
		err_code = adc_init_fn();
		err_code = dma_init_fn();
		//err_code = leuart_init_fn();
		err_code = LETimer0_Init(ENERGY_MODE);
	}

	/* Infinite loop */
	while (1)
	{
		sleep();
	}
}
