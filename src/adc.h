#ifndef HEADER_ADC
#define HEADER_ADC

unsigned int adc_init_fn(void); //ADC initialization function

#ifndef DMA_ON
void ADC0_IRQHandler(void); //ADC Interrupt Handler
#endif

#endif
