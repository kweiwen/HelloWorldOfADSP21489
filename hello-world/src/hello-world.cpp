#include <sys/platform.h>
#include <iostream>
#include "init_PLL_SDRAM.h"
#include "initSRU.h"
#include <sru.h>

void Delay_Cycles(unsigned int delayCount);

int main(void)
{
	Init_PLL();
	Init_SDRAM();
	initDAI();

	std::cout << "Hello, World!" << std::endl;

	int flag = 0;

	while(1)
	{
		Delay_Cycles(50000000);
		SRU2(HIGH, DPI_PB14_I);
		Delay_Cycles(50000000);
		SRU2(LOW, DPI_PB14_I);

		if(flag)
		{
			break;
		}
	}

	return 0;
}

void Delay_Cycles(unsigned int delayCount)
{
	/* delayCount = 1 => 38ns delay */
	while(delayCount--);
}
