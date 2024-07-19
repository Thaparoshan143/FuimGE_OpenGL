#ifndef TIMER_CPP
#define TIMER_CPP

#include<iostream>

#define DELAY_TIME 1/24.0
#define SEC_CLOCK()	((double)clock() / CLOCKS_PER_SEC)

void delay(double secs = DELAY_TIME)
{
	double end = SEC_CLOCK() + secs;
	while(SEC_CLOCK() < end);
}

#endif