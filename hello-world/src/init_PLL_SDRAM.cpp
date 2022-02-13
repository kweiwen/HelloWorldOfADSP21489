#include <cdef21489.h>
#include <def21489.h>
#include "sysreg.h"
#include "init_PLL_SDRAM.h"

void Init_PLL(void)
{
	int temp,i;

	/* Step 1 - change the PLLD to 4 */
	temp=*pPMCTL;
	temp&=~PLLD16 ;
	temp|=(PLLD4 | DIVEN) ;
	*pPMCTL = temp;

	/* Step 2 - wait for dividers to stabilize */
	for(i=0;i<16;i++);

	/* Step 3 - set INDIV bit now to bring down the VCO speed and enter the bypass mode */
	temp&=~DIVEN;
	temp|=(INDIV | PLLBP);
	*pPMCTL = temp;

	/* Step 4 - wait for the PLL to lock */
	for(i=0;i<4096;i++)

	/* Step 5 - come out of the bypass mode */
	temp=*pPMCTL;
	temp&=~PLLBP;
	*pPMCTL = temp;

	/* Step 6 - wait for dividers to stabilize */
	for(i=0;i<16;i++);

	/* Step 7 - set the required PLLM and INDIV values here and enter the bypass mode */
	/* PLLM = 16, INDIV = 0, fVCO= 2 * PLLM * CLKIN = 2 * 16 * 25 MHz = 800 MHz */
	temp = *pPMCTL;
	temp &=~ (INDIV | PLLM63);
	temp |= (PLLM16 | PLLBP);
	*pPMCTL = temp;

	/* Step 8 - wait for the PLL to lock */
	for(i=0;i<4096;i++);

	/* Step 9 - come out of the bypass mode */
	temp = *pPMCTL;
	temp&=~PLLBP;
	*pPMCTL=temp;

	/* Step 10 - wait for dividers to stabilize */
	for(i=0;i<16;i++);

	/* Step 11 - set the required values of PLLD(=2) and SDCKR(=2.5) here */
	/* fCCLK = fVCO/PLLD = 800/2 = 400 MHz, fSDCLK = fCCLK/SDCKR = 400/2.5 = 160 MHz */
	temp=*pPMCTL;
	temp&=~(PLLD16 | 0x1C0000 );
	temp|= (SDCKR2_5 | PLLD2 | DIVEN);
	*pPMCTL=temp;

	/* Step 12 - wait for the dividers to stabilize */
	for(i=0;i<16;i++);
}


void Init_SDRAM(void)
{
	/*
	 * SDRAM memory on EV-Board  - W9864G6JH-6
	 * Parameters
	 * Configuration             - 1M x 16 bits x 4 Bank
	 * CAS Latency               - 3
	 * No of Row Addressing      - A0:A11
	 * No of Column Addressing   - A0:A7
	 * No of Banks               - 4
	 * tRAS - 42ns
	 * tRCD - 15ns
	 * tRP  - 15ns
	 * tRC  - 60ns
	 * tWR  - (2CLK)
	 *
	 * For the 160 MHz case, tSDCLK = 1/160 MHz = 6.25ns
	 * CAS Latency = 3
	 * tRAS        = 42 / 6.25 = 7  (6.72)
	 * tRCD        = 15 / 6.25 = 3  (2.4)
	 * tRP         = 15 / 6.25 = 3  (2.4)
	 * tRC         = 60 / 6.25 = 10 (9.6)
	 * tWR                     = 2
	 * fSDCLK                  = 160 MHz
	 * tREF                    =  64 ms
	 * NRA                     = 2^12 = 4096
	 *
	 * RDIV = ((f SDCLK X t REF)/NRA) - (t RAS + t RP)
	 * RDIV = (((160 X 10^6 x 64 x 10^-3) / 4096) - (7 + 3))
	 * RDIV = 2630.625 = 2631 = 0xA47
	 *
	 *
	 *
	 *
	 * */

	int nValue = 0;

    *pSYSCTL |= MSEN;

    /* Mapping Bank 0 to SDRAM */
    *pEPCTL |= B0SD;
    *pEPCTL &= (~(B1SD|B2SD|B3SD));

    nValue = 0xA47;
    nValue |= SDROPT | BIT_17;	/* Enabling SDRAM read optimization */
								/* Setting the Modify to 1 */
	*pSDRRC = nValue;

	/* Programming SDRAM control register */
	nValue = 0;
	nValue |= (SDCL3|SDPSS|SDTRAS7|SDTRCD3|SDTRP3|SDTWR2|SDRAW12|SDCAW8|X16DE);
	*pSDCTL = nValue;
}

void Delay(const int n)
{
	int nDelay;
	for( nDelay = 0; nDelay < n; nDelay++)
	{
		asm("nop;");
	}
}



