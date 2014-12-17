#ifndef TIMER_H_
#define TIMER_H_


//void init_timer (void);

void delay_ms   (unsigned int t);
void delay_us   (unsigned int t);		// Kimura ‰ü•Ï
void delay_us_rx   (unsigned int t);

volatile unsigned int timer_flag;


#endif
