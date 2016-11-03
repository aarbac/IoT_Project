#include "dma.h"
#include "em_cmu.h"
#include "em_int.h"
#include "em_dma.h"
#include "dmactrl.h"
#include "LETimer.h"
#include "SiLabs_errcodes.h"
#include "sleep.h"
#include "main.h"

DMA_CB_TypeDef ADC_callback;

/*
 * @disclaimer: This routine belongs to SILABS IP.
 * @file: Example code for Internal Temperature Sensor from SILABS example codes
 * 	-> Internal temperature sensor example for EFM32LG_STK3600
 *
 * @version 3.20.5
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 * *****************************************************************************
 *
 * @brief Function to convert ADC sample to temperature in celsius
 * @param 32 bit integer adcSample
 * @Steps:
 *  1.
 */
float convertToCelsius(int32_t adcSample)
{
	float temp;
	/* Factory calibration temperature from device information page. */
	float cal_temp_0 = (float)((CAL_Device_Address & _DEVINFO_CAL_TEMP_MASK)
								>> _DEVINFO_CAL_TEMP_SHIFT);

	float cal_value_0 = (float)((ADC_Device_Address
								& _DEVINFO_ADC0CAL2_TEMP1V25_MASK)
								>> _DEVINFO_ADC0CAL2_TEMP1V25_SHIFT);

	/* Temperature gradient (from datasheet) */
	float t_grad = -6.27;

	temp = (cal_temp_0 - ((cal_value_0 - adcSample) / t_grad));

	return temp;
}

/*
 * @disclaimer: The below routine has been referred from the main_adc_scan.c to understand the initialization of the DMA.
 *
 * @file: main_adc_scan.c
 * @author: Silicon Labs
 * @version: 1.10
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details.
 *
 *******************************************************************************
 *
 * @brief DMA Initialization Function
 * @param void
 * @Steps:
 *  1. Check whether DMA_ON is defined during compilation
 *  2. Enable the Clock for DMA0
 *  3. Initialize the DMA structure, descriptor and channel config
 *  4. Slect the configuration channel as that of the ADC0
 *  5. Associate the callback in the channel config structure
 *  6. Associate the callback function DMATransferComplete to the callback of the channel config
 *  7. Call the DMA_CfgChannel to initialize it
 *  8. Configure the DMA Descriptor according to the increment size at the destination and source along with the data size
 *  9. Return error code Success
 */
unsigned int dma_init_fn(void)
{
#ifdef DMA_ON
	CMU_ClockEnable(cmuClock_DMA, true);

	DMA_Init_TypeDef       dmaInit;
	DMA_CfgDescr_TypeDef   descrCfg;
	DMA_CfgChannel_TypeDef chnlCfg;

	/* Configure general DMA issues */
	dmaInit.hprot        = 0;
	dmaInit.controlBlock = dmaControlBlock;
	DMA_Init(&dmaInit);

	/* Configure DMA channel used */
	chnlCfg.highPri   = false;
	chnlCfg.enableInt = true;
	chnlCfg.select    = DMAREQ_ADC0_SINGLE;
	chnlCfg.cb        = &ADC_callback;
	ADC_callback.cbFunc = DMAtransferComplete;
	ADC_callback.primary = true;
	ADC_callback.userPtr = NULL;
	DMA_CfgChannel(DMA_ADC_CHANNEL, &chnlCfg);

	descrCfg.dstInc  = DMA_Dest_inc;
	descrCfg.srcInc  = DMA_Src_inc;
	descrCfg.size    = DMA_data_size;
	descrCfg.arbRate = DMA_arb_rate;
	descrCfg.hprot   = 0;
	DMA_CfgDescr(DMA_ADC_CHANNEL, true, &descrCfg);
#endif
	return SILABS_SUCCESS;
}


