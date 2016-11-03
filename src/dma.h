#ifndef HEADER_DMA
#define HEADER_DMA

#include <stdbool.h>
#include <stdint.h>

unsigned int dma_init_fn(void); //DMA Initialization function
float convertToCelsius(int32_t adcSample); //Convert the ADC value to temperature equivalent in Celsius

#endif
