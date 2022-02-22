#include <sru.h>

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

    /*  Connect the AD1939 BCLKs and LRCLKs: The AD1939 drives a ABCLK output to
     * input DAI pin 7, a ALRCLK frame sync to DAI pin 8.
     *
     * Internally connect ABCLK/ALRCLK DAI pins 7/8 back out to DBCLK/DLRCLK DAI
     * pins 13 and 14, respectively
     *
     * In this example, the ADC clocks are master, DAC clocks are slaves
     *
     * ABCLK/ALRCLK are always inputs to the SHARC so tie the pin
     * buffer inputs and pin buffer enable inputs all low (already done prior, see
     * initial SRU macro instructions above).
     */

    SRU(HIGH, PBEN02_I);
    SRU(DAI_PB14_O, DAI_PB02_I);    /* Route an external clock(24.576MHz) to MCLK Bus */


    /* BCLK */

    /* LRCLK */


    SRU(DAI_PB08_O, SPORT1_CLK_I);   /* DAIP08 (ABCLK) to SPORT1 CLK (RCLK)*/
    SRU(DAI_PB08_O, SPORT0_CLK_I);   /* DAIP08 (ABCLK) to SPORT0 CLK (TCLK)*/

    SRU(DAI_PB11_O, SPORT1_FS_I);    /* DAIP11 (ALRCLK) to SPORT1 FS (RFS1)*/
    SRU(DAI_PB11_O, SPORT0_FS_I);    /* DAIP11 (ALRCLK) to SPORT0 FS (TFS0)*/

    /*  Connect the AD1939 ADCs: The AD1939 drives I2S rx ADC data to DAI pins 5
     * and 6.  We Connect the I2S ADC stream to SPORT1, using data inputs pri. A
     * and sec. B channels
     *
     * These lines are always inputs to the SHARC so tie the pin
     * buffer inputs and pin buffer enable inputs all low. (already done prior,
     * see initial SRU macro instructions above).
     */

    SRU(DAI_PB05_O, SPORT1_DA_I);	    /* DAIP5 (ASDATA1) to SPORT1 DA (RX1A) */
    SRU(DAI_PB09_O, SPORT1_DB_I);		/* DAIP9 (ASDATA2) to SPORT1 DB (RX1B) */


    /*  Connect the AD1939 DACs in I2S mode to SPORT0 and SPORT2:
     * The DAC clock and frame sync inputs are provided by the ADCs
     *
     * All eight DAC connections are always outputs from the SHARC
     * so tie the pin buffer enable inputs all high.
     *
     * Connect the I2S DAC streams to SPORT0 A/B and SPORT2 A/B
     * via DAI pins 12, 11, 10 and 9
     *
     * Connect DAC1 DSDATA1 to SPORT0, using data output A
     * Connect DAC2 DSDATA2 to SPORT0, using data output B
     * Connect DAC3 DSDATA3 to SPORT2, using data output A
     * Connect DAC4 DSDATA4 to SPORT2, using data output B
     */

    SRU(HIGH, PBEN06_I);
    SRU(SPORT0_DA_O, DAI_PB06_I);    /* DAIP06 (DSDATA1) to SPORT0 DA (TX) */
    SRU(HIGH, PBEN10_I);
    SRU(SPORT0_DB_O, DAI_PB10_I);    /* DAIP10 (DSDATA2) to SPORT0 DB (TX) */

	/* Codec Reset */
    SRU2(LOW, DPI_PB13_I);
	SRU2(HIGH, DPI_PBEN13_I);

	/* UART TX signal is connected to DPI pin 9 */
   	SRU2(UART0_TX_O, DPI_PB07_I);
	SRU2(HIGH, DPI_PBEN07_I);

	/* connect the pin buffer output signal to the UART0 RX */
	SRU2(DPI_PB08_O, UART0_RX_I);
	SRU2(LOW, DPI_PB08_I);
	/* disable DPI pin10 as input */
	SRU2(LOW, DPI_PBEN08_I);

	/* Configure LED */
	SRU2(LOW, DPI_PB14_I);
	SRU2(HIGH, DPI_PBEN14_I);
}

