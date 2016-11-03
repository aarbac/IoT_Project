#include "acmp.h"
#include "SiLabs_errcodes.h"
#include "GPIO.h"
#include "sleep.h"
#include "LETimer.h"
#include "main.h"

#include "em_acmp.h"
#include "em_gpio.h"
#include "em_int.h"
#include "em_cmu.h"

ACMP_Init_TypeDef ACMP_zero_init = ACMP_INIT_DEFAULT; //Default structure for start use

/*******
 *@brief ACMP Interrupt Handler
 *@param void
 *Steps:
 * 1. Disable all interrupts
 * 2. Clear the EDGE interrupt
 * 3. Enable all interrupts
 */
void ACMP0_IRQHandler(void)
{
	INT_Disable();
	ACMP_IntClear(ACMP0, ACMP_IF_EDGE);
	INT_Enable();
}


/*******
 *@brief ACMP Initialization Function
 *@param EMx = Energy Mode
 *Steps:
 * 1. Enable the ACMP0 clock
 * 2. Change default values in the ACMP_zero structure
 * 3. Set the VDD level, warm-up time, interrupts enable and hysteresis levels
 * 4. Initialize the ACMP0
 * 5. Set the negative and positive select channels
 * 6. Clear interrupt register and Enable the Edge Interrupt
 * 7. Enable the ACMP0 interrupt in the NVIC register
 * 8. Wait for warm-up
 * 9. Enter sleep mode
 * 10. Return err_code SUCCESS
 */
unsigned int ACMP_Init_fn(void)
{
	CMU_ClockEnable(cmuClock_ACMP0, true);
	ACMP_zero_init.vddLevel = 0x02;
	ACMP_zero_init.halfBias = true;
	ACMP_zero_init.biasProg = 0;
	ACMP_zero_init.warmTime = acmpWarmTime256;
	ACMP_zero_init.interruptOnFallingEdge = true;
	ACMP_zero_init.interruptOnRisingEdge = true;
	ACMP_zero_init.hysteresisLevel = acmpHysteresisLevel7;
	ACMP_zero_init.lowPowerReferenceEnabled = true;
	ACMP_Init(ACMP0, &ACMP_zero_init);
	ACMP_ChannelSet(ACMP0, ACMP_Channel_negative, ACMP_Channel_positive);
	while(!(ACMP0->STATUS & ACMP_STATUS_ACMPACT));
	//BlockSleepMode(ENERGY_MODE);
	return SILABS_SUCCESS;
}
