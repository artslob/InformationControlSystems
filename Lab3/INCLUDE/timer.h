#ifndef __TIMER__H
#define __TIMER__H

void InitTimer(void);
unsigned long GetMsCounter(void);
unsigned long DTimeMs(unsigned long t0);
void delay (unsigned long ms);

#endif //__TIMER__H