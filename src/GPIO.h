#ifndef HEADER_GPIO
#define HEADER_GPIO

#include "sleep.h"
#include "em_gpio.h"

unsigned int GPIO_Init(sleepstatemode_enum EMx); //GPIO Setup Function
unsigned int GPIO_on_off(unsigned char set_clear, GPIO_Port_TypeDef port, unsigned int pin_number); //Turn on, off and toggle the LED

#endif
