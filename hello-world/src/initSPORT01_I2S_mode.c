#include <platform_include.h>      /* System and IOP register bit and address definitions. */
#include "initSPORT01_I2S_mode.h"

/* Block Size per Audio Channel*/
#define NUM_SAMPLES 128

/* Number of stereo channels*/
#define NUM_RX_SLOTS 2
#define NUM_TX_SLOTS 2

#define RX_BLOCK_SIZE (NUM_SAMPLES*NUM_RX_SLOTS)
#define TX_BLOCK_SIZE (NUM_SAMPLES*NUM_TX_SLOTS)

#define PCI  0x00080000
#define OFFSET 0x00080000
#define OFFSET_MASK 0x7FFFF



// SPORT RX DMA destination buffers
int RxBlock_A0[RX_BLOCK_SIZE];
int RxBlock_A1[sizeof(RxBlock_A0)];

// SPORT Tx DMA source buffers
int TxBlock_A0[TX_BLOCK_SIZE];
int TxBlock_A1[sizeof(TxBlock_A0)];

// SPORT RX DMA destination buffers
int RxBlock_B0[RX_BLOCK_SIZE];
int RxBlock_B1[sizeof(RxBlock_B0)];

// SPORT Tx DMA source buffers
int TxBlock_B0[TX_BLOCK_SIZE];
int TxBlock_B1[sizeof(TxBlock_B0)];

//Set up the TCBs to rotate automatically
int TCB_RxBlock_A0[4] = { 0, sizeof(RxBlock_A0), 1, 0};
int TCB_RxBlock_A1[4] = { 0, sizeof(RxBlock_A0), 1, 0};

int TCB_TxBlock_A0[4] = { 0, sizeof(TxBlock_A0), 1, 0};
int TCB_TxBlock_A1[4] = { 0, sizeof(TxBlock_A0), 1, 0};

int TCB_RxBlock_B0[4] = { 0, sizeof(RxBlock_B0), 1, 0};
int TCB_RxBlock_B1[4] = { 0, sizeof(RxBlock_B0), 1, 0};

int TCB_TxBlock_B0[4] = { 0, sizeof(TxBlock_B0), 1, 0};
int TCB_TxBlock_B1[4] = { 0, sizeof(TxBlock_B0), 1, 0};

void initSPORT()
{

	// Set up "ping-pong" chained DMAs for AD1939
    //Proceed from Block A0 to Block A1
    TCB_RxBlock_A0[0] = (unsigned int) TCB_RxBlock_A1 + 3 - OFFSET + PCI ;
    TCB_RxBlock_A0[3] = (int) RxBlock_A0 - OFFSET ;

    //Proceed from Block A0 to Block A1
    TCB_TxBlock_A0[0] = (unsigned int) TCB_TxBlock_A1 + 3 - OFFSET ;
    TCB_TxBlock_A0[3] = (int) TxBlock_A0 - OFFSET ;

    //Proceed from Block A1 to Block A0
    TCB_RxBlock_A1[0] = (unsigned int) TCB_RxBlock_A0 + 3 - OFFSET + PCI ;
    TCB_RxBlock_A1[3] = (int) RxBlock_A1 - OFFSET ;

    //Proceed from Block A1 to Block A0
    TCB_TxBlock_A1[0] = (unsigned int) TCB_TxBlock_A0 + 3 - OFFSET ;
    TCB_TxBlock_A1[3] = (int) TxBlock_A1 - OFFSET ;
    
    // Set up "ping-pong" chained DMAs for AD1939
    //Proceed from Block A0 to Block A1
    TCB_RxBlock_B0[0] = (unsigned int) TCB_RxBlock_B1 + 3 - OFFSET + PCI ;
    TCB_RxBlock_B0[3] = (int) RxBlock_B0 - OFFSET ;

    //Proceed from Block A0 to Block A1
    TCB_TxBlock_B0[0] = (unsigned int) TCB_TxBlock_B1 + 3 - OFFSET ;
    TCB_TxBlock_B0[3] = (int) TxBlock_B0 - OFFSET ;

    //Proceed from Block A1 to Block A0
    TCB_RxBlock_B1[0] = (unsigned int) TCB_RxBlock_B0 + 3 - OFFSET + PCI ;
    TCB_RxBlock_B1[3] = (int) RxBlock_B1 - OFFSET ;

    //Proceed from Block A1 to Block A0
    TCB_TxBlock_B1[0] = (unsigned int) TCB_TxBlock_B0 + 3 - OFFSET ;
    TCB_TxBlock_B1[3] = (int) TxBlock_B1 - OFFSET ;
    
    //Clear out SPORT 0/1 registers
	*pSPMCTL0 = 0;
	*pSPCTL0 = 0;
	*pSPCTLN0 = 0;

	*pSPMCTL1 = 0;
    *pSPCTL1 = 0;
    *pSPCTLN1 = 0;

    /////////////////////////////////////////////////////////////////////////////////////
    //      Analog Input and output setup
    /////////////////////////////////////////////////////////////////////////////////////
   	*pDIV0 = 0x00000000;  // Transmitter (SPORT0)
    *pDIV1 = 0x00000000;  // Receiver (SPORT1) at 24.576 MHz SCLK and 96 kHz sample rate


	// Enabling DMA Chaining for SPORT0 TX/SPORT1 RX
    // Block 1 will be filled first
    
    *pCPSP0A = (unsigned int)TCB_TxBlock_A0 - OFFSET + 3 + PCI;

    *pCPSP1A = (unsigned int)TCB_RxBlock_A0 - OFFSET + 3;

    *pCPSP0B = (unsigned int)TCB_TxBlock_B0 - OFFSET + 3 + PCI;

    *pCPSP1B = (unsigned int)TCB_RxBlock_B0 - OFFSET + 3;

	// sport1 control register set up as a receiver in I2S
	// sport 1 control register SPCTL1 = 0x000C01F0
	// externally generated SCLK1 and RFS1
	*pSPCTL1 = OPMODE | SPEN_A | SCHEN_A | SDEN_A | SLEN32 | SPEN_B | SCHEN_B | SDEN_B;

	// sport0 control register set up as a transmitter in I2S
	// sport 0 control register, SPCTL0 = 0x000C01F0 */
	*pSPCTL0 = OPMODE | SPEN_A | SCHEN_A | SDEN_A | SPTRAN | SLEN32 | SPEN_B | SCHEN_B | SDEN_B;
}
