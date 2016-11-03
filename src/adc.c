#include "sleep.h"
#include "main.h"
#include "SiLabs_errcodes.h"
#include "adc.h"
#include "dma.h"
#include "GPIO.h"

#include "em_cmu.h"
#include "em_adc.h"
#include "em_int.h"
#include "em_gpio.h"

int adc_value_buffer[1000];

/*
 * @brief ADC Interrupt Handler
 * @param void
 * @Steps:
 *  1. Check whether DMA is on during compilation
 *  2. Disable all interrupts and clear the interrupt
 *  3. Put the ADC converted value into a buffer
 *  4. Increment count of the buffer
 *  5. Get it out of EM1
 *  6. Check whether count is equal to the sample count i.e 1000
 *  7. Stop the ADC conversion
 *  8. Add all the ADC sample values in the buffer
 *  9. Calculate the average ADC sample value
 *  10. Convert it to temperature equivalent in Celsius
 *  11. Check whether the converted temperature is within limits
 *	12. Depending on the above outcome, switch on or switch off the LED
 *	13. Empty the buffer and make count back to zero
 *	14. Enable all the interrupts
 */
#ifndef DMA_ON
#ifndef USE_POLLING
void ADC0_IRQHandler(void)
{
	int32_t average_temp = 0;
	float temp_ret = 0.0;
	static int count;
	INT_Disable();
	adc_value_buffer[count] = ADC_DataSingleGet(ADC0);
	count++;
	if(count == ADC_sample_count)
	{
		UnBlockSleepMode(EM1);
		ADC0->CMD |= ADC_CMD_SINGLESTOP;
		for(int i = 0; i < ADC_sample_count; i++)
		{
			average_temp += adc_value_buffer[i];
		}
		average_temp = (int32_t)average_temp/1000;
		temp_ret = convertToCelsius(average_temp);

		if(temp_ret < Temperature_Lower_Limit || temp_ret > Temperature_Upper_Limit)
		{
			GPIO_on_off(0, LED_pin_port, LED1_pin_number);
		}
		else
		{
			GPIO_on_off(1, LED_pin_port, LED1_pin_number);
		}

		count = 0;
		for(int i = 0; i < 1000; i++)
		{
			adc_value_buffer[i] = 0;
		}
	}
	ADC_IntClear(ADC0, ADC_IF_SINGLE);
	INT_Enable();
}
#endif
#endif

/*
 * @disclaimer: The below routine has been referred from the main_adc_single.c to understand the initialization of the ADC for single conversion.
 *
 * @file: main_adc_single.c
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
 * @brief ADC Initialization Function
 * @param void
 * @Steps:
 *  1. Enable the clock for ADC0 and High Frequency
 *  2. Define ADC initialize and ADC single conversion structures
 *  3. Calculate prescaler for ADC0 frequency
 *  4. Calculate the timebase for ADC0
 *  5. Initialize the ADC
 *  6. Check whether the DMA is on during compilation and if not enabled then enable the Single conversion Interrupt
 *  7. Define the input channel which is the internal temperature sensor to the ADC
 *  8. Define the reference for the ADC single conversion which is 1.25V
 *  9. Define the resolution for the ADC single conversion which is 12 bits
 *  10. Define the acquisition time for the ADC Single Conversion
 *  11. Enable the ADC to do continuous conversion till explicitly mentioned stop
 *  12. Initialize the ADC for single conversion
 *  13. Return error code success
 */
unsigned int adc_init_fn(void)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_ADC0, true);

	ADC_Init_TypeDef adc_init =
			{                                                                             \
			  adcOvsRateSel2,                /* 2x oversampling (if enabled). */          \
			  adcLPFilterBypass,             /* No input filter selected. */              \
			  adcWarmupNormal,               /* ADC shutdown after each conversion. */    \
			  _ADC_CTRL_TIMEBASE_DEFAULT,    /* Use HW default value. */                  \
			  _ADC_CTRL_PRESC_DEFAULT,       /* Use HW default value. */                  \
			  false                          /* Do not use tailgate. */                   \
			};

	ADC_InitSingle_TypeDef adc_single =
			{                                                                                 \
			  adcPRSSELCh0,              /* PRS ch0 (if enabled). */                          \
			  adcAcqTime1,               /* 1 ADC_CLK cycle acquisition time. */              \
			  adcRef1V25,                /* 1.25V internal reference. */                      \
			  adcRes12Bit,               /* 12 bit resolution. */                             \
			  adcSingleInpCh0,           /* CH0 input selected. */                            \
			  false,                     /* Single ended input. */                            \
			  false,                     /* PRS disabled. */                                  \
			  false,                     /* Right adjust. */                                  \
			  false                      /* Deactivate conversion after one scan sequence. */ \
			};

	adc_init.prescale = ADC_PrescaleCalc(ADC_FREQ, 0);
	adc_init.timebase = ADC_TimebaseCalc(0);

#ifndef DMA_ON
#ifndef USE_POLLING
	ADC_IntClear(ADC0, ADC0->IFC);
	ADC0->IEN |= ADC_IEN_SINGLE;
	NVIC_EnableIRQ(ADC0_IRQn);
#endif
#endif
	ADC_Init(ADC0, &adc_init);

	adc_single.input = ADC_input_channel;
	adc_single.reference = ADC_ref;
	adc_single.resolution = ADC_resolution;
	adc_single.acqTime = ADC_acquisition_time;

#ifdef USE_POLLING
	adc_single.rep = false;
#else
	adc_single.rep = true;
#endif

	ADC_InitSingle(ADC0, &adc_single);
	return SILABS_SUCCESS;
}
