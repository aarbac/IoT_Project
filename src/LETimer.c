#include "sleep.h"
#include "LETimer.h"
#include "GPIO.h"
#include "SiLabs_errcodes.h"
#include "acmp.h"
#include "clock.h"
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "leuart.h"
#include "captouch.h"
#include "LCD.h"
#include "rtc.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "em_int.h"
#include "em_letimer.h"
#include "em_timer.h"
#include "em_acmp.h"
#include "em_adc.h"
#include "em_dma.h"
#include "em_i2c.h"
#include "em_leuart.h"
#include "em_lesense.h"
#include "em_rtc.h"

unsigned int ACMP_Out_flag;
unsigned int adc_count = 0;
volatile int16_t ADC_data_buffer[1000];
unsigned int led_flag = 0;
unsigned int energymode_flag = 0;
unsigned int cap_flag = 0;
extern unsigned int captouch_interrupt_flags;

/*
 * @brief ACMP_zero structure which will be used to change the VDD scaling factor
 */
ACMP_Init_TypeDef ACMP_zero = \
		{
	   	  false,              /* fullBias */                                       \
		  true,              /* halfBias */                                       \
		  0,                /* biasProg */                                       \
		  true,              /* No interrupt on falling edge. */                   \
		  true,              /* No interrupt on rising edge. */                    \
		  acmpWarmTime256,    /* 256 cycle warm-up to be safe */                    \
		  acmpHysteresisLevel7,                                                    \
		  false,              /* Disabled emiting inactive value during warm-up. */\
		  true,              /* low power reference */                            \
		  0x02,               /* VDD level */                                      \
		  true                /* Enable after init. */                             \
		};


/*****
 *@brief LETimer0 Initialization function
 *@param EMx = Energy Mode
 *Steps:
 *	1. Make LETIMER initialize to default values
 *	2. Clear the Interrupt Flag Registers
 *	3. Enable Compare 0 interrupt
 *	4. Check for Energy Mode
 *	5. Put the calculated value in the Compare registers
 *	6. If Energy Mode is not EM3, then Enable Underflow Interrupt
 *	7. Initialize the LETimer with the above values
 *	8. Enable the LETIMER Interrupt in the NVIC
 *	9. Enable the LETIMER
 */
unsigned int LETimer0_Init(sleepstatemode_enum EMx)
{
	int count_val, prescaler;
	LETIMER_Init_TypeDef LETIMER_zero = LETIMER_INIT_DEFAULT;

	prescaler = 2;
	prescaler = ((prescaler & 0x0000000f) << 8);
	CMU->LFAPRESC0 |= prescaler;

	LETIMER_IntClear(LETIMER0, LETIMER0->IFC);
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP0);

	if(EMx == EM3)
	{
#ifdef SELF_CALIBRATION
		count_val = ((int)(LED_OFF_SECS * self_calibration_ratio));
		LETIMER_CompareSet(LETIMER0, 0, count_val);
#else
		count_val = ((int)(LED_OFF_SECS * ULFRCO_FREQ));
		LETIMER_CompareSet(LETIMER0, 0, count_val);
#endif
		//LETIMER_zero.comp0Top = true;
		LETIMER_CompareSet(LETIMER0, 0, 0xFFFF-count_val); //generate interrupt for LED off time
	}
	else if(EMx == EM0 || EMx == EM1 || EMx == EM2)
	{
		count_val = 0xFFFF - ( LFXO_FREQ_PRESCALER_BY_4 * (LED_OFF_SECS * 1000) / 1000 );
		LETIMER_CompareSet(LETIMER0, 0, count_val); //generate interrupt for LED toggle
	}
	else
	{
		return SILABS_BAD_PARAM;
	}
	LETIMER_Init( LETIMER0, &LETIMER_zero);
	NVIC_EnableIRQ(LETIMER0_IRQn);
	LETIMER_Enable(LETIMER0,true);
	BlockSleepMode(ENERGY_MODE);
	return SILABS_SUCCESS;
}

/*******
 *@brief LETimer Interrupt Handler
 *@param void
 *Steps:
 *	1. All Interrupts Disable
 *	2. Determine Pending Interrupts
 *	3. Clear the Interrupts which occurred
 *	4. Check for the ACMP0_OUT bit and check whether it is 0 or 1
 *	5. Check for secs_flag for deciding new count in LETIMER0->COMP0 value
 *	6. For secs_flag = 0, change value of flag, Enable the ACMP0, Set the Excite pin, Clear the CNT register, Load 4ms count in COMP0
 *	7. For secs_flag = 1, change value of flag, Disable the ACMP0, Set the Excite pin, Clear the CNT register, Load 3secs count in COMP0
 *	8. Check whether ACMP is excited and ACMP_OUT bit is set or cleared for changing the VDD scaling in the ACMP register and toggling the LED
 *	9. Initialize the ACMP0 with the structure
 *	10. Set the negative and positive select Channels because they get cleared when ACMP0 is initialized
 *	12. For secs_flag = 1, put device in EM1
 *	13. Check whether DMA_ON is called during compilation, if true, activate DMA
 *	14. Start the ADC conversion
 *	15. All Interrupts Enable
 */
void LETIMER0_IRQHandler(void)
{
	static int secs_flag = 0;
	unsigned int cnt_val = 0;
	static uint8_t TSL2651_period_flag = 0;
	INT_Disable();
	BlockSleepMode(ENERGY_MODE);
	LETIMER_IntClear(LETIMER0,LETIMER_IF_COMP0);

	ACMP_Out_flag = ((ACMP0->STATUS & ACMP_STATUS_ACMPOUT) >> _ACMP_STATUS_ACMPOUT_SHIFT);

	if(secs_flag == 0)
	{
		secs_flag = 1;
#ifdef ENABLE_ON_BOARD_LIGHT_SENSOR
		ACMP_Enable(ACMP0);
		GPIO_PinOutSet(Light_Excite_port, Light_Excite_number);
#endif
		LETIMER0->CNT = 0;
		if(ENERGY_MODE == EM3)
		{
			energymode_flag = 1;
#ifdef SELF_CALIBRATION
			cnt_val = ((int)(LED_ON_SECS * self_calibration_ratio));
			if(self_calibration_ratio < 1000)
			{
				cnt_val = cnt_val/4;
				cnt_val++;
			}
			//LETIMER_CompareSet(LETIMER0, 0, 0xFFFF - (cnt_val/2));
			LETIMER0->COMP0 = 0xFFFF - cnt_val;
#else
			cnt_val = ((int)(LED_ON_SECS * ULFRCO_FREQ));
			LETIMER_CompareSet(LETIMER0, 0, 0xFFFF - (cnt_val/2));
#endif
#ifndef ENABLE_ON_BOARD_LIGHT_SENSOR
		if(TSL2651_period_flag == 0)
		{
			//BlockSleepMode(EM1);
			//tsl2651_enable();
			TSL2651_period_flag = 1;
		}
		else if(TSL2651_period_flag == 1)
		{
			TSL2651_period_flag = 2;
			cap_flag = 1;
		}
		else if(TSL2651_period_flag == 2)
		{
			BlockSleepMode(EM3);
			//tsl2651_disable();
			//UnBlockSleepMode(EM1);
			TSL2651_period_flag = 0;
			cap_flag = 0;
		}
#endif
		}
		else
		{
			energymode_flag = 0;
#ifndef ENABLE_ON_BOARD_LIGHT_SENSOR
		if(TSL2651_period_flag == 0)
		{
			//BlockSleepMode(EM1);
			//tsl2651_enable();
			TSL2651_period_flag = 1;
		}
		else if(TSL2651_period_flag == 1)
		{
			TSL2651_period_flag = 2;
		}
		else if(TSL2651_period_flag == 2)
		{
			BlockSleepMode(EM3);
			//tsl2651_disable();
			//UnBlockSleepMode(EM1);
			TSL2651_period_flag = 0;
		}
#endif
			LETIMER_CompareSet(LETIMER0, 0, (0xFFFF - ( LFXO_FREQ_PRESCALER_BY_4 * (LED_ON_SECS * 1000) / 1000)));
		}
	}
	else if(secs_flag == 1)
	{
		secs_flag = 0;
#ifdef ENABLE_ON_BOARD_LIGHT_SENSOR
		ACMP_Disable(ACMP0);
		GPIO_PinOutClear(Light_Excite_port, Light_Excite_number);
#endif
		LETIMER0->CNT = 0;
		if(ENERGY_MODE == EM3)
		{
#ifdef SELF_CALIBRATION
			cnt_val = ((int)(LED_OFF_SECS * self_calibration_ratio));
			//LETIMER_CompareSet(LETIMER0, 0, 0xFFFF - (cnt_val/2));
			LETIMER0->COMP0 = 0xFFFF - (cnt_val/4);
#else
			cnt_val = ((int)(LED_OFF_SECS * ULFRCO_FREQ));
			LETIMER_CompareSet(LETIMER0, 0, 0xFFFF - (cnt_val/2));
#endif
		}
		else
		{
			LETIMER_CompareSet(LETIMER0, 0, (0xFFFF - ( LFXO_FREQ_PRESCALER_BY_4 * (LED_OFF_SECS * 1000) / 1000)));
		}
	}
#ifdef ENABLE_ON_BOARD_LIGHT_SENSOR
	if(secs_flag == 0 && ACMP_Out_flag == 1)
	{
		GPIO_on_off(1, LED_pin_port, LED0_pin_number);
		ACMP_zero.vddLevel = ACMP_Darkness_threshold;
		ACMP_Init(ACMP0, &ACMP_zero);
		ACMP_ChannelSet(ACMP0, ACMP_Channel_negative, ACMP_Channel_positive);
	}
	else if(secs_flag == 0 && ACMP_Out_flag == 0)
	{
		GPIO_on_off(0, LED_pin_port, LED0_pin_number);
		ACMP_zero.vddLevel = ACMP_Light_threshold;
		ACMP_Init(ACMP0, &ACMP_zero);
		ACMP_ChannelSet(ACMP0, ACMP_Channel_negative, ACMP_Channel_positive);
	}
#endif

	if(secs_flag == 1)
	{
		BlockSleepMode(EM1);
#ifdef DMA_ON
		DMA_ActivateBasic(DMA_ADC_CHANNEL, true, false, (void *) ADC_data_buffer, (void *) &(ADC0->SINGLEDATA),(ADC_sample_count - 1));
#endif
		ADC_Start(ADC0, adcStartSingle);

#ifdef USE_POLLING
		int32_t temp_decimal = 0;
		int average_temp = 0;
		float temp_ret = 0.0;
		while(adc_count != ADC_sample_count)
		{
			ADC_Start(ADC0, adcStartSingle);
			while (ADC0->STATUS & ADC_STATUS_SINGLEACT) ;
			average_temp += ADC_DataSingleGet(ADC0);
			adc_count++;
		}
		average_temp = average_temp/ADC_sample_count;
		temp_ret = convertToCelsius(average_temp);
		average_temp = 0; adc_count = 0;
		if(temp_ret < Temperature_Lower_Limit || temp_ret > Temperature_Upper_Limit)
		{
			GPIO_on_off(0, LED_pin_port, LED1_pin_number);
		}
		else
		{
			GPIO_on_off(1, LED_pin_port, LED1_pin_number);
		}
		temp_decimal = (int32_t)(temp_ret*10) % 10;
		led_flag = GPIO_PinOutGet(LED_pin_port, LED0_pin_number);
		if(energymode_flag == 0)
		{
			leuart_tx_data((uint8_t)temp_ret);
			leuart_tx_data((uint8_t)temp_decimal);
			leuart_tx_data((uint8_t)led_flag);
		}
		else if(energymode_flag == 1)
		{
			leuart_tx_data((uint8_t)temp_ret);
			for(int i = 0; i < 2000; i++);
			leuart_tx_data((uint8_t)temp_decimal);
			for(int i = 0; i < 2000; i++);
			leuart_tx_data((uint8_t)led_flag);
			for(int i = 0; i < 2000; i++);
		}
		UnBlockSleepMode(EM1);
#endif
	}
	INT_Enable();
}


/*
 *@brief DMA Callback Function indicating DMA Transfer is complete
 *@param unsigned integer channel, boolean primary and void point user
 *Steps:
 *	1. Stop the ADC conversion
 *	2. Get out of EM1 mode
 *	3. Add all the temperature values from the ADC buffer
 *	4. Find the average value of temperature
 *	5. Convert the average adc value into its respective Celsius equivalent
 *	6. Check whether the converted temperature is within limits
 *	7. Depending on the above outcome, switch on or switch off the LED
 */
void DMAtransferComplete(unsigned int channel, bool primary, void *user)
{
	int32_t temp_decimal = 0;
	int32_t average_temp = 0;
	float temp_ret = 0.0;
	ADC0->CMD |= ADC_CMD_SINGLESTOP;
	UnBlockSleepMode(EM1);
	for(int i = 0; i < ADC_sample_count; i++)
	{
		average_temp += ADC_data_buffer[i];
	}
	average_temp = (int32_t)average_temp/ADC_sample_count;
	temp_ret = convertToCelsius(average_temp);

	if(temp_ret < Temperature_Lower_Limit || temp_ret > Temperature_Upper_Limit)
	{
		GPIO_on_off(0, LED_pin_port, LED1_pin_number);
	}
	else
	{
		GPIO_on_off(1, LED_pin_port, LED1_pin_number);
	}

	temp_decimal = (uint32_t)(temp_ret*10) % 10;
	led_flag = GPIO_PinOutGet(LED_pin_port, LED0_pin_number);
	leuart_tx_data((int8_t)temp_ret);
	leuart_send(temp_decimal, led_flag);
#ifdef LEUART_TEST_ON
	if(energymode_flag == 0)
	{
		leuart_tx_data((int8_t)temp_ret);
		leuart_tx_data((uint8_t)temp_decimal);
		leuart_tx_data((uint8_t)led_flag);
	}
	else if(energymode_flag == 1)
	{
		leuart_tx_data((int8_t)temp_ret);
		for(int i = 0; i < 2000; i++);
		leuart_tx_data((uint8_t)temp_decimal);
		for(int i = 0; i < 2000; i++);
		leuart_tx_data((uint8_t)led_flag);
	}
#endif
#if(0)
	static uint8_t count = 0;
	if(count == 0)
	{
		leuart_tx_data(50);
		//for(int i = 0; i < 10000; i++);
		count++;
	}
	else if(count == 1)
	{
		leuart_tx_data(20);
		//for(int i = 0; i < 10000; i++);
		count++;
	}
	else if(count == 2)
	{
		leuart_tx_data(15);
		for(int i = 0; i < 10000; i++);
		count++;
	}
	else if(count == 3)
	{
		leuart_tx_data(10);
		for(int i = 0; i < 10000; i++);
		count++;
	}
	else if(count == 4)
	{
		leuart_tx_data(10);
		for(int i = 0; i < 10000; i++);
		count++;
	}
	else if(count == 5)
	{
		leuart_tx_data(10);
		for(int i = 0; i < 10000; i++);
		count++;
		count = 0;
	}
#endif
}

/*******
 *@brief LETimer Calibrate Routine
 *@param void
 *Steps:
 *	1. Enable the LFXO oscillator
 *	2. Select the LFA clock tree
 *	3. Enable the CORELE and LETimer0 Clocks
 *	4. Initialize the LETimer structure
 *	5. Load the count in the CNT register for down counting wrt the LFXO
 *	6. Enable LETIMER, TIMER0, TIMER1
 *	7. Wait for LETIMER0->CNT register to become zero which indicates 1 second
 *	8. Calculate the count value from TIMER0 and TIMER1 CNT registers
 *	9. Disable the TIMER0 and TIMER1 and clear the registers
 *	10. Disable the LFXO and Enable the ULFRCO oscillator
 *	11. Load the count in the CNT register for down counting wrt the ULFRCO
 *	12. Repeat step 6, 7 and 8
 *	13. Calculate oscillator calibration ratio by dividing the LFXO count by the ULFRCO count
 *	14. Return err_code SUCCESS
 */
unsigned int LETimer_Calibrate(void)
{
	unsigned int ratio_temp;
	unsigned int timer_cnt_val1 = 0;
	unsigned int LEtimer_cnt_val1 = 0;
	unsigned int timer_cnt_val2 = 0;
	unsigned int LEtimer_cnt_val2 = 0;
	CMU_OscillatorEnable(cmuOsc_LFXO,true,true);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_LETIMER0,true);
	LETIMER_Init_TypeDef LETIMER_INIT = LETIMER_INIT_DEFAULT;
	LETIMER_INIT.comp0Top = true;
	LETIMER_INIT.enable = false;
	LETIMER_Init(LETIMER0, &LETIMER_INIT);
	LETIMER0->CNT = 0x8000;
	LETIMER_Enable(LETIMER0, true);
	TIMER_Enable(TIMER0,true);
	TIMER_Enable(TIMER1,true);
	LEtimer_cnt_val1 = LETIMER0->CNT;
	while(LEtimer_cnt_val1 !=0)
	{
		LEtimer_cnt_val1 = LETIMER0->CNT;
	}
	timer_cnt_val1 = (TIMER1->CNT * 65536) + TIMER0->CNT;
	TIMER_Enable(TIMER0, false);
	TIMER_Enable(TIMER1, false);
	TIMER0->CNT = 0;
	TIMER1->CNT = 0;

	CMU_OscillatorEnable(cmuOsc_LFXO,false,true);
	CMU_OscillatorEnable(cmuOsc_ULFRCO,true,true);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);
	LETIMER0->CNT = 1000;
	LETIMER_Enable(LETIMER0, true);
	TIMER_Enable(TIMER0,true);
	TIMER_Enable(TIMER1,true);
	LEtimer_cnt_val2 = LETIMER0->CNT;
	while(LEtimer_cnt_val2 !=0)
	{
		LEtimer_cnt_val2 = LETIMER0->CNT;
	}
	timer_cnt_val2 = (TIMER1->CNT * 65536) + TIMER0->CNT;

	ratio_temp = ((float)timer_cnt_val1/timer_cnt_val2) * 1000;

	self_calibration_ratio = ratio_temp;

	TIMER_Enable(TIMER0,false);
	TIMER_Enable(TIMER1,false);
	CMU_OscillatorEnable(cmuOsc_ULFRCO,false,true);
	CMU_ClockEnable(cmuClock_CORELE, false);
	CMU_ClockEnable(cmuClock_LETIMER0,false);

	return SILABS_SUCCESS;
}

void LESENSE_IRQHandler( void )
{
	INT_Disable();
	static uint16_t count = 0;
	uint16_t channels_touched = 0;
	static uint16_t buttons_pressed;
	uint8_t channel, i, valid_touch;
	uint32_t interrupt_flags, tmp, channels_enabled;
	uint16_t threshold_value;

	count++;
	if(count == 3)
	{
		count = 0;
		INT_Enable();
		return;
	}

	captouch_interrupt_flags = LESENSE->IEN;

	/* Get interrupt flag */
	interrupt_flags = LESENSE_IntGet();
	/* Clear interrupt flag */
	LESENSE_IntClear(interrupt_flags);

	/* Interrupt handles only one channel at a time */
	/* therefore only first active channel found is handled by the interrupt. */
	for(channel = 0; channel < NUM_LESENSE_CHANNELS; channel++)
	{
		if( (interrupt_flags >> channel) & 0x1 )
		{
		  break;
		}
	}

	/* To filter out possible false touches, the suspected channel is measured several times */
	/* All samples should be below threshold to trigger an actual touch. */

	/* Disable other channels. */
	channels_enabled = LESENSE->CHEN;
	LESENSE->CHEN = 1 << channel;

	/* Evaluate VALIDATE_CNT results for touched channel. */
	valid_touch = 1;

	for(i = 0;i<VALIDATE_CNT;i++)
	{
		/* Start new scan and wait while active. */
		LESENSE_ScanStart();
		while(LESENSE->STATUS & LESENSE_STATUS_SCANACTIVE);

		tmp = LESENSE->SCANRES;
		if((tmp & (1 << channel)) == 0)
		{
		  valid_touch = 0;
		}
	}

	/* Enable all channels again. */
	LESENSE->CHEN = channels_enabled;


	if(valid_touch)
	{
		/* If logic was switched clear button flag and set logic back, else set button flag and invert logic. */
		if(LESENSE->CH[channel].EVAL & LESENSE_CH_EVAL_COMP)
		{
		  buttons_pressed &= ~(1 << channel);
		  LESENSE->CH[channel].EVAL &= ~LESENSE_CH_EVAL_COMP;

		  threshold_value = LESENSE->CH[channel].EVAL & (_LESENSE_CH_EVAL_COMPTHRES_MASK);
		  /* Change threshold value 1 LSB for hysteresis. */
		  threshold_value -= 1;
		  LESENSE_ChannelThresSet(channel, 0, threshold_value);
		}
		else
		{
		  buttons_pressed |= (1 << channel);
		  LESENSE->CH[channel].EVAL |= LESENSE_CH_EVAL_COMP;

		  threshold_value = LESENSE->CH[channel].EVAL & (_LESENSE_CH_EVAL_COMPTHRES_MASK);
		  /* Change threshold value 1 LSB for hysteresis. */
		  threshold_value += 1;
		  LESENSE_ChannelThresSet(channel, 0, threshold_value);
		}
	}

	channels_touched = buttons_pressed;

	if(channels_touched)
	{
		GPIO_on_off(0, LED_pin_port, LED0_pin_number);
		//LCD_write_string("CAP_ON");
		//flag_cap = 1;
		//send_status(channels_touched);
		//RTC_CounterReset();
	}
	else
	{
		GPIO_on_off(1, LED_pin_port, LED0_pin_number);
		//LCD_write_string(" ");
		//flag_cap = 0;
		//LESENSE_IntEnable(LESENSE->IEN);
		//GPIO_on_off(1, LED_pin_port, LED0_pin_number);
	}
	INT_Enable();
}


