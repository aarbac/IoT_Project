#include "GPIO.h"
#include "sleep.h"
#include "LETimer.h"
#include "SiLabs_errcodes.h"
#include "main.h"

#include <stdbool.h>
#include "em_cmu.h"
#include "em_emu.h"
#include "em_int.h"
#include "em_gpio.h"

/*******
 *@brief GPIO Initialization Function
 *@param EMx = Energy Mode
 *Steps:
 *	1. Enable the Clock for GPIO
 *	2. Set, Clear or Disable the respective Pin using PinModeSet
 *	3. Check for Energy mode EM3, if yes then set the GPIO LED0 pin
 *	4. Return err_code SUCCESS
 */
unsigned int GPIO_Init(sleepstatemode_enum EMx)
{
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(Light_Sense_port, Light_Sense_number, gpioModeDisabled, 0); //Light sense pin

	GPIO_PinModeSet(LED_pin_port, LED0_pin_number, gpioModePushPull, 0); //LED0 pin

	GPIO_PinModeSet(LED_pin_port, LED1_pin_number, gpioModePushPull, 0); //LED0 pin

#ifdef ENABLE_ON_BOARD_LIGHT_SENSOR
	GPIO_PinModeSet(Light_Excite_port, Light_Excite_number, gpioModePushPull, 1); //Light excite pin
#endif

	GPIO_PinModeSet(I2C_port, I2C_SCL_pin_number, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(I2C_port, I2C_SDA_pin_number, gpioModeWiredAnd, 1);

	GPIO_PinModeSet(I2C_Power_port, I2C_Interrupt_pin, gpioModeInput, 0);
	GPIO_PinModeSet(I2C_Power_port, I2C_Power_pin, gpioModePushPull, 0);

	GPIO_DriveModeSet(LED_pin_port, gpioDriveModeLowest);
	GPIO_DriveModeSet(Light_Excite_port, gpioDriveModeLowest);


	GPIO_PinModeSet(LEUART0_port, LEUART0_TX_pin_number, gpioModePushPull, 1);
	GPIO_PinModeSet(LEUART0_port, LEUART0_RX_pin_number, gpioModePushPull, 1);

	return SILABS_SUCCESS;
}

/*******
 *@brief User Defined LED GPIO Set/Clear Function
 *@param Set_Clear: Used to set/clear/toggle the respective GPIO
 *@param port: Corresponding GPIO port
 *@param pin_number: Corresponding pin number on the port on which the action is to be performed
 *Steps:
 *	1. Check for set_clear
 *	2. Set or Clear or Toggle the GPIO LED
 *	3. Return err_code SUCCESS or BAD PARAMETER
 */
unsigned int GPIO_on_off(unsigned char set_clear, GPIO_Port_TypeDef port, unsigned int pin_number)
{
	if(set_clear == 0)
	{
		GPIO_PinOutSet(port, pin_number);
	}
	else if(set_clear == 1)
	{
		GPIO_PinOutClear(port, pin_number);
	}
	else if(set_clear == 2)
	{
		GPIO_PinOutToggle(port, pin_number);
	}
	else
	{
		return SILABS_BAD_PARAM;
	}
	return SILABS_SUCCESS;
}
