#include <sys/platform.h>
#include <iostream>
#include "init_PLL_SDRAM.h"
#include "initSRU.h"
#include <sru.h>
#include <services/int/adi_int.h>  			/* Interrupt Handler API header. */
#include <sru21489.h>						/* Get definition of SRU() */
#include <stdlib.h>							/* Get definition of NULL */
#include "UART0_isr.h"
#include "ADDS_21489_EzKit.h"


char TEST[] =
{
		"\f"
		"TEST!"
		"\n"
};

void Delay_Cycles(unsigned int delayCount);

int main(void)
{
	Init_PLL();
	Init_SDRAM();
	initDAI();

	/* init UART and install interrupt */
	initUART();
    adi_int_InstallHandler(ADI_CID_P14I, UART0ISR, NULL, true);
	adi_int_InstallHandler(ADI_CID_TMZLI, TimerISR, (void *)&timer_isr_count, true);

	std::cout << "Hello, World!" << std::endl;

	int flag = 0;
	int buffer_cntr = 0;


	while(1)
	{
		Delay_Cycles(10000000);
		SRU2(HIGH, DPI_PB14_I);
		Delay_Cycles(10000000);
		SRU2(LOW, DPI_PB14_I);

		Delay_Cycles(1000000);
		DBG(TEST);
		char temp = buffer_cntr + '0';
		DBG(&temp);
		Delay_Cycles(1000000);

		buffer_cntr++;

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
