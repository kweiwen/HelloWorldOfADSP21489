#include <sys/platform.h>
#include <iostream>
#include "ADDS_21489_EzKit.h"
#include "init_PLL_SDRAM.h"
#include "initSRU.h"
#include <sru.h>
#include <services/int/adi_int.h>
#include "hw_connect.h"
#include "UART0_isr.h"
#include "initSPORT01_I2S_mode.h"
#include "SPORT1_isr.h"
#include "blockProcess_audio.h"
#include "init_PCG.h"
#include "AT.h"

void Delay_Cycles(unsigned int delayCount);
void Codec_Reset(void);

void initUART(void);
void Init_UART_Baud(int baud,int Len,char Party,int Stop);



char welcomemessage[] = {"\fHello! \n\r"
                  "This program echoes back whatever you type in...\n\r"};



extern volatile int timer_isr_count;
extern volatile int commandReady;
extern volatile float CoreLoad;
extern char uart_buffer[];
extern volatile int uart_rx_len;
extern float rms_ch1;;
char X;
int main(void)
{
 	Init_PLL();
	Init_SDRAM();
	Init_PCG();
	initDAI();
	/* init UART and install interrupt */
	initUART();
	adi_int_InstallHandler(ADI_CID_P14I, UART0ISR, NULL, true);
	adi_int_InstallHandler(ADI_CID_TMZLI, TimerISR, (void *)&timer_isr_count, true);

	initSPORT();
	adi_int_InstallHandler(ADI_CID_P3I, TalkThroughISR, 0, true);

	_Codec_Reset_L();
	Delay_Cycles(10000000);//248ms,10000000 = 248ms @ 400MHZ CCLK*/
	_Codec_Reset_H();

	std::cout << "Hello, World!" << std::endl;
	DBG(welcomemessage);

	int flag = 0;

	while(1)
	{
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
    		std::cout << "commandReady" << std::endl;

    		char str[128];
    		//sprintf(str, "CoreLoad=%0.2f%%", CoreLoad*100);
    		//sprintf(str, "ch1 input=%0.5fV",rms_ch1);
    		//xmitUARTmessage_DMA(str, strlen(str));

			//xmitUARTmessage_DMA(uart_buffer, uart_rx_len);
    		X = at_cmd_parse(uart_buffer, uart_rx_len);
    		commandReady = 0;
    		uart_rx_len = 0;
    		printf("at_cmd_parse = %x\n",X);
    	}
	}
	return 0;
}

void Delay_Cycles(unsigned int delayCount)
{
	/* delayCount = 1 => 24.8nS delay @ 400MHZ CCLK*/
	while(delayCount--);
}


void Codec_Reset(void)
{
	/* delayCount = 1 => 24.8nS delay @ 400MHZ CCLK*/
    _Codec_Reset_H();
    Delay_Cycles(5);
	_Codec_Reset_L();
    Delay_Cycles(5);//need��  40ns
    _Codec_Reset_H();
}
