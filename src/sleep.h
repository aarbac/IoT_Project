#ifndef HEADER_SLEEP
#define HEADER_SLEEP

/*
 * @brief Used to define the Energy modes
 */
typedef enum
{
	EM0 = 0,
	EM1,
	EM2,
	EM3,
	EM4
}sleepstatemode_enum;

void sleep(void); //Used for Entering respective energy mode
void BlockSleepMode(sleepstatemode_enum minimumMode); //Sets the respective energy mode in the sleep counter array
void UnBlockSleepMode(sleepstatemode_enum minimumMode); //Clears the respective energy mode in the sleep counter array

#endif
