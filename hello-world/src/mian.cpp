#include <sys/platform.h>
#include <iostream>
#include "init_PLL_SDRAM.h"
#include "initSRU.h"
#include <sru.h>
#include <services/int/adi_int.h>
#include "Goble_Define.h"
#include "hw_connect.h"
#include "UART0_isr.h"
#include "initSPORT01_I2S_mode.h"
#include "SPORT1_isr.h"
#include "blockProcess_audio.h"
#include "init_PCG.h"


void Delay_Cycles(unsigned int delayCount);
void Codec_Reset(void);

void initUART(void);
void Init_UART_Baud(int baud,int Len,char Party,int Stop);



char welcomemessage[] = {"\fHello! \n\r"
                  "This program echoes back whatever you type in...\n\r"};



extern volatile int timer_isr_count;
extern volatile int commandReady;
extern char uart_buffer[];
int main(void)
{
 	Init_PLL();
	Init_SDRAM();
	Init_PCG();
	initDAI();
	_Codec_Reset_H();
	/* init UART and install interrupt */
	initUART();
	adi_int_InstallHandler(ADI_CID_P14I, UART0ISR, NULL, true);
	adi_int_InstallHandler(ADI_CID_TMZLI, TimerISR, (void *)&timer_isr_count, true);

	initSPORT();
	adi_int_InstallHandler(ADI_CID_P3I, TalkThroughISR, 0, true); //Programmable Interrupt 3 - SPORT1 ,sport1 control register set up as a receiver in I2S
	//adi_int_InstallHandler(ADI_CID_P6I, TalkThroughISR, 0, true);//Programmable Interrupt 3 - SPORT0

	std::cout << "Hello, World!" << std::endl;
	DBG(welcomemessage);

	_Codec_Reset_L();
	Delay_Cycles(10000000);//248ms,100000000 = 2480ms£¬24.8nS per Cycle
	_Codec_Reset_H();

	int flag = 0;

	while(1)
	{
		//Delay_Cycles(50000000);
		//Run_LED_Blink();
		//Delay_Cycles(50000000);
		//Run_LED_Blink();

		if(flag)
		{
			break;
		}

    	if(inputReady)
    	{
    		handleCodecData(buffer_cntr);
    	}

    	if(commandReady)
    	{
			xmitUARTmessage(uart_buffer, 8);
    		commandReady = 0;
    	}
	}

	return 0;
}

void Delay_Cycles(unsigned int delayCount)
{
	/* delayCount = 1 => 24.8nS delay */
	while(delayCount--);
}


void Codec_Reset(void)
{
	/* delayCount = 1 => 38ns delay */
    _Codec_Reset_H();
    Delay_Cycles(5);
	_Codec_Reset_L();
    Delay_Cycles(5);//need¡¶  40ns
    _Codec_Reset_H();
}
