/*¾§Õñ(18.432MHz)*/
#include <STC15.h>	    
#include "delay.h"
void Delay1us()		//@18.432MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void Delay1ms()		//@18.432MHz
{
	unsigned char i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}

void DelayMS(int t)
{
	while(t--)
	{
		Delay1ms();
	}
}

void DelayUS(int t)
{
	while(t--)
	{
		Delay1us();
	}
}



