#include "initSRU.h"
#include <sru.h>
#include <sysreg.h>

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
    SRU(LOW, PBEN01_I);
    SRU(LOW, PBEN02_I);
    SRU(LOW, PBEN03_I);
    SRU(LOW, PBEN04_I);
    SRU(LOW, PBEN05_I);
    SRU(LOW, PBEN06_I);
    SRU(LOW, PBEN07_I);
    SRU(LOW, PBEN08_I);
    SRU(LOW, PBEN09_I);
    SRU(LOW, PBEN10_I);
    SRU(LOW, PBEN11_I);
    SRU(LOW, PBEN12_I);
    SRU(LOW, PBEN13_I);
    SRU(LOW, PBEN14_I);
    SRU(LOW, PBEN15_I);
    SRU(LOW, PBEN16_I);
    SRU(LOW, PBEN17_I);
    SRU(LOW, PBEN18_I);
    SRU(LOW, PBEN19_I);
    SRU(LOW, PBEN20_I);
}


void initDAI(void)
{
    clearDAIpins();

	SRU(FLAG4_O, DPI_PB14_I);
	SRU(HIGH, DPI_PBEN14_I);
	/* setting flag pins as outputs */
	sysreg_bit_set( sysreg_FLAGS, (FLG4O) );
	/* clearing flag pins */
	sysreg_bit_clr( sysreg_FLAGS, (FLG4O) );


	sysreg_bit_clr(sysreg_FLAGS, FLG4);
//	sysreg_bit_set(sysreg_FLAGS, FLG4);

//	{
//		if( LED_OFF == bState ) 	{ sysreg_bit_clr(sysreg_FLAGS, FLG4); }
//		else if( LED_ON == bState )	{ sysreg_bit_set(sysreg_FLAGS, FLG4); }
//		else	/* toggle */		{ sysreg_bit_tgl(sysreg_FLAGS, FLG4); }
//	}
}

