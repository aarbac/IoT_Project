#ifndef HEADER_LETIMER
#define HEADER_LETIMER

#include "sleep.h"
#include <stdbool.h>

unsigned int LETimer0_Init(sleepstatemode_enum EMx); //LETimer Setup Function
void LETIMER0_IRQHandler(void); //LETimer Interrupt Handler
unsigned int LETimer_Calibrate(void); //Calibrate the ULFRCO function
void DMAtransferComplete(unsigned int channel, bool primary, void *user); //DMA callback function
void LESENSE_IRQHandler( void );

unsigned int self_calibration_ratio;

#endif
