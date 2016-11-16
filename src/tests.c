#include "tests.h"
#include "GPIO.h"
#include "LCD.h"
#include "captouch.h"
#include "main.h"
#include "SiLabs_errcodes.h"

#include "em_int.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_lesense.h"
#include "em_lcd.h"
#include "em_rtc.h"

unsigned int test_LCD(void)
{
	LCD_write_string("LCD_ON");
	for(int i = 0; i < 1000000; i++);
	LCD_write_string(" ");
	return SILABS_SUCCESS;
}

unsigned int test_LESENSE_captouch(void)
{
	capsense_main();
	return SILABS_SUCCESS;
}

__STATIC_INLINE void led_alert_low(void)
{
	RTC_CompareSet( 0, ALERT_LVL_LOW_SECS * RTC_FREQ );
	for(int i = 0; i < 10000000; i++);
}

__STATIC_INLINE void led_alert_medium(void)
{
	RTC_CompareSet( 0, ALERT_LVL_MEDIUM_SECS * RTC_FREQ );
	for(int i = 0; i < 10000000; i++);
}

__STATIC_INLINE void led_alert_high(void)
{
	RTC_CompareSet( 0, ALERT_LVL_HIGH_SECS * RTC_FREQ );
	for(int i = 0; i < 10000000; i++);
}

unsigned int test_led(unsigned int alert_lvl)
{
	if(alert_lvl == LOW)
	{
		led_alert_low();
	}
	else if(alert_lvl == MEDIUM)
	{
		led_alert_medium();
	}
	else if(alert_lvl == HIGH)
	{
		led_alert_high();
	}
	GPIO_on_off(1, LED_pin_port, LED0_pin_number);
	RTC_CompareSet( 0, CALIBRATION_INTERVAL * RTC_FREQ );
	RTC_CounterReset();
	return SILABS_SUCCESS;
}
