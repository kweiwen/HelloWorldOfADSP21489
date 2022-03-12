#include "UART0_isr.h"
#include <cdef21489.h>
#include <def21489.h>
#include <stdio.h>					/* Get declaration of puts and definition of NULL. */
#include <assert.h>    				/* Get the definition of support for standard C asserts. */
#include <services/int/adi_int.h>  	/* Interrupt Handler API header. */
#include <stdint.h>    /* Get definition of uint32_t. */


#ifndef UART_BUFFER_SIZE
	#define UART_BUFFER_SIZE (16)
#endif

#ifndef UART_BUFFER_MASK
	#define UART_BUFFER_MASK (UART_BUFFER_SIZE - 1)
#endif

#ifndef PCLK
	#define PCLK	200e6
#endif


char uart_buffer[UART_BUFFER_SIZE] = {0};
volatile int commandReady = 0;
volatile int isCommandProcessing = 0;
volatile int uart_buffer_cntr = 0;
volatile int uart_buffer_ready = 0;
volatile int timer_isr_count = 0;
volatile int uart_rx_len = 0;
static void Init_UART_Baud(int baud,int Len,char Party,int Stop);

char welcomeMessage[] =
{
		"\f"
		"Welcome!"
		"\n"
};

char errorMessage[] =
{
		"\f"
		"ERROR!"
		"\n"
		"UART controller is not able to collect enough data in time,"
		" message buffer will be clear."
		"\n"
};

void initUART(void)
{
   	*pPICR2 &= ~(0x7C00); /* Sets the UART0 receive interrupt to P14 */
   	*pPICR2 |= (0x13<<10);

   	*pUART0LCR=0;

    *pUART0IER= UARTRBFIE;  /* enables UART0 receive interrupt */

    Init_UART_Baud(115200, 8,'N',1);/* Sets the Baud rate, word length, parity, stop bits for UART0 */

	/* Sets the Baud rate for UART0 */
	//*pUART0LCR = UARTDLAB;  /* enables access to Divisor register to set baud rate */

	/* Baud rate = PCLK/(16 * divisor), divisor value can be from 1 to 65,536 */
	/* 200Mhz = PCLK */
	/* 0x01B0 = 432 for divisor value and gives a baud rate of 19200 for core clock 200MHz */
	/* 0x006C = 108 for divisor value and gives a baud rate of 115200 for core clock 200MHz */
	/* 0x000E = 14 for divisor value and gives a baud rate of 576000 for core clock 200MHz */
	//*pUART0DLL = 0x6c	;//108
    //*pUART0DLH = 0x00;

    /* Configures UART0 LCR */
    /* word length 8, parity enable ODD parity, Two stop bits */
    //*pUART0LCR = UARTWLS8 | UARTPEN | UARTSTB;

    *pUART0RXCTL = UARTEN;       /* enables UART0 in receive mode */
    *pUART0TXCTL = UARTEN;       /* enables UART0 in core driven mode */

    //DBG(welcomeMessage);
}

void xmitUARTmessage(char*xmit, int SIZE)
{
	int i;

	/* loop to transmit source array in core driven mode */
	for (i=0; i<SIZE; i++)
	{
		/* Wait for the UART transmitter to be ready */
		while ((*pUART0LSR & UARTTHRE) == 0)
		{
			/* Loop until condition */
			asm("nop;");
		}

		/* Transmit a byte */
		*pUART0THR = xmit[i];
	}

	/* poll to ensure UART has completed the transfer */
	while ((*pUART0LSR & UARTTEMT) == 0)
	{
		/* Loop until condition */
		asm("nop;");
	}
}


void xmitUARTmessage_DMA(char*xmit, int SIZE)
{
	//執行一次DMA發送需要大約消耗400ns,不論字長
	*pUART0TXCTL = 0;
	*pIIUART0TX = (unsigned int)xmit;
	*pIMUART0TX = 1;
	*pCUART0TX = SIZE;
	*pUART0TXCTL = UARTEN | UARTDEN;
};


#pragma optimize_for_speed /* interrupt handlers usually need to be optimized */
#pragma section ("seg_int_code")  /* handler functions perform better in internal memory */
void UART0ISR(uint32_t iid, void *handlerArg)
{
	/* UART interrupt service routine. */

	/* Don't call standard I/O functions or update
	 * non-volatile global data in interrupt handlers.
	 */

	/* You can use the handler arguments to
	 * identify the interrupt being handled (iid)
	 * and to access data via an interrupt specific
	 * callback pointer argument (handlerArg).
	 */
	assert(iid == ADI_CID_P14I);
	assert(handlerArg == NULL);

	int value;
	if(*pUART0LSR & UARTDR)
	{
		/* clean buffer at the first time, invert the status logic to prevent re-write */
		if(uart_buffer_ready==0)
		{
			for(int i=0; i<UART_BUFFER_SIZE; i++)
			{
				uart_buffer[i] = 0;
			}
			uart_buffer_ready=1;
		}

		/* assign register value to variable and store in the buffer array */
		value = *pUART0RBR;
		uart_buffer[uart_buffer_cntr++] = value;
		uart_buffer_cntr &= UART_BUFFER_MASK;
		uart_rx_len = uart_buffer_cntr;

		/* once the buffer is full transmit the message to UART TX */
		/*
		if(uart_buffer_cntr==0)
		{
			timer_off();
			uart_buffer_ready = 0;
			timer_isr_count = 0;
			commandReady = 1;
		}
		else
		{
			// Set tperiod and tcount of the timer.
			timer_set(1000, 1000);
			// Start the timer.
			timer_on();
		}
		*/
		timer_set(1000, 1000);
		timer_on();
	}
} /* UART0ISR */

#pragma optimize_for_speed /* interrupt handlers usually need to be optimized */
#pragma section ("seg_int_code")  /* handler functions perform better in internal memory */
void TimerISR(uint32_t iid, void* handlerArg)
{
	/* Timer interrupt service routine. */

	/* Don't call standard I/O functions or update
	 * non-volatile global data in interrupt handlers.
	 */

	/* You can use the handler arguments to
	 * identify the interrupt being handled (iid)
	 * and to access data via an interrupt specific
	 * callback pointer argument (handlerArg).
	 */
	assert(iid == ADI_CID_TMZHI || iid == ADI_CID_TMZLI);
	assert(handlerArg != NULL);

	volatile int *timer_counter = (int *) handlerArg;
	*timer_counter += 1;

	if(*timer_counter > 4096)
	{
		timer_off();
		uart_buffer_ready = 0;
		uart_buffer_cntr = 0;
		timer_isr_count = 0;
		commandReady = 1;
		/* we should add an error flag here */
	}

} /* TimerISR */

void DBG(char* input_data)
{
	int index = 0;

	while(input_data[index] != '\0')
	{
		index++;
	}
	//xmitUARTmessage(input_data, index+1);
	xmitUARTmessage_DMA(input_data, index+1);
}


static void Init_UART_Baud(int baud,int Len,char Party,int Stop)
{
	int UART_CLK = ((float)PCLK/16);

	int UART0LCR_Temp = 0;
	int DLL = (UART_CLK/baud)&0x00ff;
	int DLH = ((UART_CLK/baud)&0xff00)<<8;

	// Sets the Baud rate for UART0
	//The fundamental timing clock of the UART module is peripheral clock/16 (PCLK/16).
	*pUART0LCR = UARTDLAB;  //enables access to Divisor register to set bauda rate
	*pUART0DLL = DLL;        //0x1B0 = 432 for divisor value and gives a baud rate of 19200 for peripheral clock of 133MHz
    *pUART0DLH = DLH;

    //�ֳ�
    switch (Len){
    case 5:
    	UART0LCR_Temp |= UARTWLS5; //word length 5
    	break;
    case 6:
    	UART0LCR_Temp |= UARTWLS6; //word length 6
    	break;
    case 7:
    	UART0LCR_Temp |= UARTWLS7; //word length 7
    	break;
    case 8:
    	UART0LCR_Temp |= UARTWLS8; //word length 8
    	break;
    default:
    	UART0LCR_Temp |= UARTWLS8; //word length 8
    	break;
    };

    //У��λ
    switch (Party){
    case 'N':
    	UART0LCR_Temp &=  (~UARTPEN);
    	break;
    case 'O':
    	UART0LCR_Temp |= UARTPEN; 				// parity enable ODD parity
    	UART0LCR_Temp &=  (~UARTEPS);
    	break;
    case 'E':
    	UART0LCR_Temp |= UARTPEN; 				// parity enable Even parity
    	UART0LCR_Temp |= UARTEPS;
    	break;
    default:
    	UART0LCR_Temp &=  (~UARTPEN);
    	break;
    };

    //ֹͣλ
    switch (Stop){
    case 1:
    	UART0LCR_Temp &=  (~UARTSTB); 	         // One stop bits
    	break;
    case 2:
    	UART0LCR_Temp |=  UARTSTB; 				// Two stop bits
    	break;
    default:
    	UART0LCR_Temp &=  (~UARTSTB); 	         // One stop bits
    	break;
    };

    *pUART0LCR = UART0LCR_Temp;
}

