#ifndef HEADER_CLOCK
#define HEADER_CLOCK

#include "sleep.h"

unsigned int Clock_Init(sleepstatemode_enum EMx); //CMU Setup Function
unsigned int Timer_Init_fn(void); //Timer0 and Timer1 Initialize function
void TIMER0_IRQHandler(void); //Timer0 Interrupt Handler

#endif
