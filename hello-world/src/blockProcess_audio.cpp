#include "blockProcess_audio.h"
#include "ADDS_21489_EzKit.h"
#include "hw_connect.h"
#include <sru21489.h>
#include <math.h>
#include <cycle_count.h>	//For Basic Cycle Count
#include <cycles.h>		//For Cycle Count With Statistics
#include <time.h> 		//For Cycle Count Using Time
#include <stdio.h>


// Structures to hold floating point data for each AD1939
ad21479_float_data_1 fBlockA;
ad21479_float_data_2 fBlockB;

// SPORT Ping/Pong Data buffers
extern int TxBlock_A0[];
extern int TxBlock_A1[];

extern int RxBlock_A0[];
extern int RxBlock_A1[];

extern int TxBlock_B0[];
extern int TxBlock_B1[];

extern int RxBlock_B0[];
extern int RxBlock_B1[];

//Pointer to the blocks
int *rx_block_pointer[2] = {RxBlock_A0, RxBlock_A1};
int *tx_block_pointer[2] = {TxBlock_A0, TxBlock_A1};
int *rx_block_pointer2[2] = {RxBlock_B0, RxBlock_B1};
int *tx_block_pointer2[2] = {TxBlock_B0, TxBlock_B1};

// Unoptimized function to convert the incoming fixed-point data to 32-bit floating-point format.
// This function assumes that the incoming fixed point data is in 1.31 format
void floatData(float *output, int *input, unsigned int instep, unsigned int length)
{
    int i;

    for(i = 0; i < length; i++)
    {
        output[i] = __builtin_conv_RtoF(input[instep*i]);
    }
}

// Unoptimized function to convert the outgoing floating-point data to 1.31 fixed-point format.
void fixData(int *output, float *input, unsigned int outstep, unsigned int length)
{
    int i;

    for(i = 0; i < length; i++)
    {
        output[outstep*i] = __builtin_conv_FtoR(input[i]);
    }
}

// Unoptimized function to copy from one floating-point buffer to another
void memcopy(float *input, float *output, unsigned int number)
{
    int i;
    
    for(i = 0; i < number; i++)
    {
        output[i] = input[i];
    }
}

void process_audioBlocks()
{
	/* ADC to DAC */
    memcopy(fBlockA.Rx_L1, fBlockA.Tx_L1, NUM_SAMPLES);
	memcopy(fBlockA.Rx_R1, fBlockA.Tx_R1, NUM_SAMPLES);
    memcopy(fBlockB.Rx2_L1, fBlockB.Tx2_L1, NUM_SAMPLES);
	memcopy(fBlockB.Rx2_R1, fBlockB.Tx2_R1, NUM_SAMPLES);

	/* doing something takes amount of time here */
	for(int i = 0; i < 8192; i++)
	{
	//	float a = (i + 1) * (i + 1);
	//	float b = sqrtf(a);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// This function handles the Codec data in the following 3 steps...
// 1. Converts all ADC data to 32-bit floating-point, and copies this 
//    from the current RX DMA buffer into fBlockA & fBlockB
// 2. Calls the audio processing function (processBlocks)
// 3. Converts all DAC to 1.31 fixed point, and copies this from 
//    fBlockA & fBlockB into the current TX DMA buffer
/////////////////////////////////////////////////////////////////////////////////////

void handleCodecData(unsigned int blockIndex)
{
	 volatile clock_t clock_start;
	 volatile clock_t clock_stop;
	 double secs;
	 float percent;
	_Run_LED_On();
	clock_start = clock();

    //Clear the Block Ready Semaphore
    inputReady = 0;
    //Set the Processing Active Semaphore before starting processing
    isProcessing = 1;

    // Float ADC data from Codec
	floatData(fBlockA.Rx_L1, rx_block_pointer[blockIndex]+0, NUM_RX_SLOTS, NUM_SAMPLES);
	floatData(fBlockA.Rx_R1, rx_block_pointer[blockIndex]+1, NUM_RX_SLOTS, NUM_SAMPLES);
	floatData(fBlockB.Rx2_L1, rx_block_pointer2[blockIndex]+0, NUM_RX_SLOTS, NUM_SAMPLES);
	floatData(fBlockB.Rx2_R1, rx_block_pointer2[blockIndex]+1, NUM_RX_SLOTS, NUM_SAMPLES);

	// Place the audio processing algorithm here. 
	process_audioBlocks();

    // Fix DAC data for Codec
	fixData(tx_block_pointer[blockIndex]+0, fBlockA.Tx_L1, NUM_TX_SLOTS, NUM_SAMPLES);
	fixData(tx_block_pointer[blockIndex]+1, fBlockA.Tx_R1, NUM_TX_SLOTS, NUM_SAMPLES);
	fixData(tx_block_pointer2[blockIndex]+0, fBlockB.Tx2_L1, NUM_TX_SLOTS, NUM_SAMPLES);
	fixData(tx_block_pointer2[blockIndex]+1, fBlockB.Tx2_R1, NUM_TX_SLOTS, NUM_SAMPLES);

    //Clear the Processing Active Semaphore after processing is complete
    isProcessing = 0;

    clock_stop = clock();
    _Run_LED_Off();
    secs = ((double) ((clock_stop - clock_start)) / CLOCKS_PER_SEC);
    percent = secs / total_block_time_sec;

    //printf("Processing Time = %0.2fus\n",secs*1e6);
    //printf("Total Block Time = %0.2fus\n",total_block_time_sec*1e6);
    printf("DSP Load = %0.2f%%\n",percent*100);

}

