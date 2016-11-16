#ifndef HEADER_RTC
#define HEADER_RTC

void LETOUCH_setupRTC(void);
void RTC_IRQHandler(void);
void send_status(unsigned int channels);

#endif
