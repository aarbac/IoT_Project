#include "rtc.h"
#include "GPIO.h"
#include "main.h"

#include "em_rtc.h"
#include "em_gpio.h"
#include "em_lesense.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_int.h"

unsigned int test_flag;
unsigned int captouch_interrupt_flags;

unsigned int temp = 0;

void send_status(unsigned int channels)
{
	temp = channels;
}

void RTC_IRQHandler( void )
{
	INT_Disable();

	/* Clear interrupt flag */
	RTC_IntClear(RTC_IFS_COMP0);

	if(temp)
	{
	  LESENSE_IntEnable(captouch_interrupt_flags);
	  temp = 0;
	}
	else if(temp == 0)
	{

	}

	if(test_flag == 1)
	{
#ifdef TESTS_ON
		GPIO_on_off(2, LED_pin_port, LED0_pin_number);
#endif
	}

	/* Reset counter */
	RTC_CounterReset();
	INT_Enable();
}

void LETOUCH_setupRTC( void )
{
	CMU_ClockEnable(cmuClock_RTC, true);
	/* RTC configuration */
	static const RTC_Init_TypeDef rtcInit =
	{
	.enable   = true,
	.debugRun = false,
	.comp0Top = true
	};

	RTC_Init(&rtcInit);

	RTC_CompareSet(0, CALIBRATION_INTERVAL * RTC_FREQ);

	RTC_IntEnable(RTC_IFS_COMP0);
	NVIC_EnableIRQ(RTC_IRQn);
}
