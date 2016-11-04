#ifndef HEADER_CAPTOUCH
#define HEADER_CAPTOUCH

#include <stdint.h>
//void LESENSE_IRQHandler(void);

void LETOUCH_Init(float sensitivity[]);
uint16_t LETOUCH_GetChannelsTouched(void);
uint16_t LETOUCH_GetChannelMaxValue(uint8_t channel);
uint16_t LETOUCH_GetChannelMinValue(uint8_t channel);
void LETOUCH_Calibration(void);

/** Scan frequency for LESENSE, how often all the pads are scanned. */
#define LESENSE_SCAN_FREQUENCY          5

/** Sample delay, how long the rc-oscillations are sampled. */
#define SAMPLE_DELAY                   30

/** Validate count is the number of consecutive scan-cycles a button needs to */
/** be in the changed state before an actual button press or release is acknowledged. */
#define VALIDATE_CNT                   10

/** Number of calibration events used to calculate threshold. */
#define NUMBER_OF_CALIBRATION_VALUES    10

/** Interval between calibration, in seconds. */
#define CALIBRATION_INTERVAL            5

#define NUM_LESENSE_CHANNELS    16

#define LESENSE_CH_PORT         gpioPortC



#endif
