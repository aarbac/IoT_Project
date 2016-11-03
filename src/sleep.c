/******************************************************************************
 * @author: Silicon Labs
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details.
 *
 * *****************************************************************************
 *
 * @disclaimer: All routines belong to SILABS IP.
 * @file: sleep.c
 * -> All routines belong to SILABS and have been taken from sleep.c
 */

#include <stdbool.h>
#include "em_emu.h"
#include "em_int.h"
#include "sleep.h"
#include "SiLabs_errcodes.h"
#include "main.h"

int sleep_block_counter[4] = {0,0,0,0}; //Sleep Block Counter Array

/*
 * @brief Sleep Function
 * @param void
 * Steps:
 * 	1. Check for sleep block counter variable
 * 	2. Enter respective Energy mode
 */
void sleep(void)
{
	if(sleep_block_counter[0] > 0)
	{
		return;
	}
	else if(sleep_block_counter[1] > 0)
	{
		EMU_EnterEM1();
	}
	else if(sleep_block_counter[2] > 0)
	{
		EMU_EnterEM2(true);
	}
	else if(sleep_block_counter[3] > 0)
	{
		EMU_EnterEM3(true);
	}
	else
	{
		//EMU_EnterEM4();
	}
}

/*****
 *@brief Block Sleep Mode function
 *@param minimum mode = Energy Mode
 *Steps:
 *	1. Interrupts Disable
 *	2. Increment the respective sleep block counter variable for respective Energy mode
 *	3. Interrupts Enable
 */
void BlockSleepMode(sleepstatemode_enum minimumMode)
{
	INT_Disable();
	sleep_block_counter[minimumMode]++;
	INT_Enable();
}

/*****
 *@brief Unblock Sleep Mode function
 *@param minimum mode = Energy Mode
 *Steps:
 *	1. Interrupts Disable
 *	2. Decrement the respective sleep block counter variable for respective Energy mode
 *	3. Interrupts Enable
 */
void UnBlockSleepMode(sleepstatemode_enum minimumMode)
{
	INT_Disable();
	if(sleep_block_counter[minimumMode] > 0)
	{
		sleep_block_counter[minimumMode]--;
	}
	INT_Enable();
}

