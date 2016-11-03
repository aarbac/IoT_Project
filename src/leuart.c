#include "leuart.h"
#include "LETimer.h"

#include "sleep.h"
#include "main.h"
#include "SiLabs_errcodes.h"

#include "em_cmu.h"
#include "em_int.h"
#include "em_leuart.h"

#include <stdint.h>
#include <stdbool.h>

int32_t decimal_val;
int32_t led_val;
unsigned int global;

unsigned int leuart_init_fn(void)
{
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_LEUART0, true);

	LEUART_Reset(LEUART0);
	LEUART_Init_TypeDef leuart_init =
	{
	  leuartEnable,      /* Enable RX/TX when init completed. */                                \
	  0,                 /* Use current configured reference clock for configuring baudrate. */ \
	  9600,              /* 9600 bits/s. */                                                     \
	  leuartDatabits8,   /* 8 data bits. */                                                      \
	  leuartNoParity,    /* No parity. */                                                       \
	  leuartStopbits2    /* 1 stop bit. */                                                       \
	};
	LEUART_Init(LEUART0, &leuart_init);

	LEUART0->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | LEUART_ROUTE_LOCATION_LOC0;
	LEUART0->CTRL |= LEUART_CTRL_LOOPBK;
	LEUART_IntEnable(LEUART0, LEUART_IEN_TXC);
	NVIC_EnableIRQ(LEUART0_IRQn);

	return SILABS_SUCCESS;
}

void leuart_tx_data(int8_t tx_data)
{
	LEUART_Tx(LEUART0, tx_data);
}

void leuart_send(int decimal, int led)
{
	decimal_val = decimal;
	led_val = led;
	global = 1;
}

void LEUART0_IRQHandler(void)
{
	static int flag = 0;
	INT_Disable();
	LEUART_IntClear(LEUART0, LEUART_IFC_TXC);
	if(global)
	{
		if(flag == 0)
		{
			LEUART0->TXDATA = decimal_val;
			flag = 1;
		}
		else if(flag == 1)
		{
			LEUART0->TXDATA = led_val;
			flag = 0;
			global = 0;
		}
	}
	INT_Enable();
}
