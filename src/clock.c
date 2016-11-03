#include "clock.h"
#include "SiLabs_errcodes.h"
#include "sleep.h"
#include "main.h"

#include "em_cmu.h"
#include "em_emu.h"
#include "em_timer.h"
#include "em_int.h"


/*****
 *@brief Clock Initialization function
 *@param EMx = Energy Mode
 *Steps:
 *	1. Check for Energy Mode
 *	2. Oscillator Enable- LFXO or ULFRCO
 *	3. Clock Select Set- LFXO or ULFRCO
 *	4. Clock Enable CORELE and LETIMER0
 */
unsigned int Clock_Init(sleepstatemode_enum EMx)
{
	if(EMx == EM0 || EMx == EM1 || EMx == EM2)
	{
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
	}
	else if(EMx == EM3)
	{
		CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
	}
	else
	{
		return SILABS_BAD_PARAM;
	}
	CMU_ClockEnable(cmuClock_CORELE, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true);

	return SILABS_SUCCESS;
}

/*****
 *@brief Timer0 Interrupt Handler
 *@param void
 *Steps:
 *	1. Disable all Interrupts
 *	2. Clear the Overflow interrupt
 *	3. Enable all Interrupts
 */
void TIMER0_IRQHandler(void)
{
	INT_Disable();
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
	INT_Enable();
}

/*****
 *@brief Timer0 and Timer1 initialize function which can be used for Calibration of ULFRCO
 *@param void
 *Steps:
 * 1. Enable High Freq clock, Timer0 and Timer1 clocks
 * 2. Write to structures Timer_zero and Timer_one for initialization purposes
 *@Note: Use Cascade Clock for Timer1 so that Timers 0 and 1 act as a 32 bit counter
 * 3. Initialize the Timer0 and Timer1 with their respective structures
 * 4. Clear the Timer0 Interrupt register
 * 5. Enable the Overflow Interrupt for Timer0
 * 6. Enable the Timer0 interrupt in the NVIC register
 * 7. Return err_code SUCCESS
 */
unsigned int Timer_Init_fn(void)
{
#ifdef SELF_CALIBRATION
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);
	TIMER_Init_TypeDef Timer_zero =
	{
	  false,                   /* Enable timer when init complete. */                      \
	  false,                  /* Stop counter during debug halt. */                       \
	  timerPrescale1,         /* No prescaling. */                                        \
	  timerClkSelHFPerClk,    /* Select HFPER clock. */                                   \
	  timerInputActionNone,   /* No action on falling input edge. */                      \
	  timerInputActionNone,   /* No action on rising input edge. */                       \
	  timerModeUp,            /* Up-counting. */                                          \
	  false,                  /* Do not clear DMA requests when DMA channel is active. */ \
	  false,                  /* Select X2 quadrature decode mode (if used). */           \
	  false,                  /* Disable one shot. */                                     \
	  false                    /* Started/stopped/reloaded by other timers. */      	      \
	};
	TIMER_Init_TypeDef Timer_one =
	{
	  false,                   /* Enable timer when init complete. */                      \
	  false,                  /* Stop counter during debug halt. */                       \
	  timerPrescale1,         /* No prescaling. */                                        \
	  timerClkSelCascade,    /* Select HFPER clock. */                                   \
	  timerInputActionNone,   /* No action on falling input edge. */                      \
	  timerInputActionNone,   /* No action on rising input edge. */                       \
	  timerModeUp,            /* Up-counting. */                                          \
	  false,                  /* Do not clear DMA requests when DMA channel is active. */ \
	  false,                  /* Select X2 quadrature decode mode (if used). */           \
	  false,                  /* Disable one shot. */                                     \
	  true                    /* Started/stopped/reloaded by other timers. */      	      \
	};
	TIMER_Init(TIMER0, &Timer_zero);
	TIMER_Init(TIMER1, &Timer_one);
#endif
	return SILABS_SUCCESS;
}

