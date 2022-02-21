#include "initSRU.h"
#include <sru.h>
#include <Cdef21489.h>

static void clearDAIpins(void)
{
/*
 *  Tie the pin buffer inputs LOW for all DAI pins.  Even though
 *  these pins are inputs to the SHARC, tying unused pin buffer inputs
 *  LOW is "good coding style" to eliminate the possibility of
 *  termination artifacts internal to the IC.  Note that signal
 *  integrity is degraded only with a few specific SRU combinations.
 *  In practice, this occurs VERY rarely, and these connections are
 *  typically unnecessary.  This is GROUP D
 */
    SRU(LOW, DAI_PB01_I);
    SRU(LOW, DAI_PB02_I);
    SRU(LOW, DAI_PB03_I);
    SRU(LOW, DAI_PB04_I);
    SRU(LOW, DAI_PB05_I);
    SRU(LOW, DAI_PB06_I);
    SRU(LOW, DAI_PB07_I);
    SRU(LOW, DAI_PB08_I);
    SRU(LOW, DAI_PB09_I);
    SRU(LOW, DAI_PB10_I);
    SRU(LOW, DAI_PB11_I);
    SRU(LOW, DAI_PB12_I);
    SRU(LOW, DAI_PB13_I);
    SRU(LOW, DAI_PB14_I);
    SRU(LOW, DAI_PB15_I);
    SRU(LOW, DAI_PB16_I);
    SRU(LOW, DAI_PB17_I);
    SRU(LOW, DAI_PB18_I);
    SRU(LOW, DAI_PB19_I);
    SRU(LOW, DAI_PB20_I);

/*
 * Tie the pin buffer enable inputs LOW for all DAI pins so
 * that they are always input pins.  This is GROUP F.
 */
    SRU2(LOW, PBEN01_I);
    SRU2(LOW, PBEN02_I);
    SRU2(LOW, PBEN03_I);
    SRU2(LOW, PBEN04_I);
    SRU2(LOW, PBEN05_I);
    SRU2(LOW, PBEN06_I);
    SRU2(LOW, PBEN07_I);
    SRU2(LOW, PBEN08_I);
    SRU2(LOW, PBEN09_I);
    SRU2(LOW, PBEN10_I);
    SRU2(LOW, PBEN11_I);
    SRU2(LOW, PBEN12_I);
    SRU2(LOW, PBEN13_I);
    SRU2(LOW, PBEN14_I);
    SRU2(LOW, PBEN15_I);
    SRU2(LOW, PBEN16_I);
    SRU2(LOW, PBEN17_I);
    SRU2(LOW, PBEN18_I);
    SRU2(LOW, PBEN19_I);
    SRU2(LOW, PBEN20_I);
}


void initDAI(void)
{

    clearDAIpins();
    SRU(LOW, DAI_PB02_I);//DAI_P2 24.576MHZ Audio CLK OSC Input

    SRU(LOW, DAI_PB09_I);//CN1 SDATA0 Input A/B
    SRU(LOW, DAI_PB05_I);//CN2 SDATA1 Input C/D

    SRU(HIGH,PBEN14_I);//MCLK Output

    //MCLK
    SRU(DAI_PB02_O, DAI_PB14_I);
    SRU(DAI_PB02_O, PCG_EXTA_I);  //—°‘ÒÕ‚≤ø ±÷”‘¥ ‰»ÎµΩPGC£¨24.576MHZ crystal to PCG_EXTA_I (CLK)

    //BCLK's
    SRU(HIGH,PBEN08_I);//BLCK Output
    SRU (PCG_CLKA_O, DAI_PB08_I);//6.144MHZ
    SRU (PCG_CLKA_O, SPORT0_CLK_I);
    SRU (PCG_CLKA_O, SPORT1_CLK_I);
    SRU (PCG_CLKA_O, SPORT2_CLK_I);

    //LRCLK's
    SRU(HIGH,PBEN11_I);//LRCK Output
    SRU (PCG_FSA_O, DAI_PB11_I);
    SRU (PCG_FSA_O, SPORT0_FS_I);
    SRU (PCG_FSA_O, SPORT0_FS_I);
    SRU (PCG_FSA_O, SPORT0_FS_I);

    //SDATA IN sport1 control register set up as a receiver in I2S
    SRU(DAI_PB09_O, SPORT1_DA_I);	//SDATA0 CN1≤Â≤€£¨IN A/B
    SRU(DAI_PB05_O, SPORT1_DB_I);	//SDATA1 CN2≤Â≤€£¨IN C/D

    //SDATA OUT sport0 control register set up as a transmitter
    SRU(HIGH, PBEN06_I);			//SDATA2 CN3≤Â≤€£¨OUT1/2
    SRU(SPORT0_DA_O, DAI_PB06_I);   /*SPORT1 DA (TX) */
    SRU(HIGH, PBEN10_I);			//SDATA3 CN4≤Â≤€£¨OUT1/2
    SRU(SPORT0_DB_O, DAI_PB10_I);   /*SPORT0 DB (TX) */


    //RUN LED
	SRU2(LOW, DPI_PB14_I);
	SRU2(HIGH, DPI_PBEN14_I);

	//CDDEC RESET
    SRU2(LOW, DPI_PB13_I);
	SRU2(HIGH, DPI_PBEN13_I);

	//UART0
	SRU2(UART0_TX_O,DPI_PB09_I); // UART transmit signal is connected to DPI pin8 /MOSI
	SRU2(HIGH,DPI_PBEN09_I);

	SRU2(DPI_PB10_O,UART0_RX_I); // connect the pin buffer output signal to the UART0 receive / MISO
	SRU2(LOW,DPI_PB10_I);
	SRU2(LOW,DPI_PBEN10_I);      // disables DPI pin7 as input

	/*
    SRU(LOW, DPI_PB08_I);
	SRU(HIGH, DPI_PBEN08_I);

    SRU(LOW, DPI_PB07_I);
	SRU(HIGH, DPI_PBEN07_I);
	*/

}


