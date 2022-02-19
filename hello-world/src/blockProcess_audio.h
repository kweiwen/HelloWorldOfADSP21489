#pragma once

/* Block Size per Audio Channel*/
#define NUM_SAMPLES 128

/* Number of stereo channels*/
#define NUM_RX_SLOTS 2
#define NUM_TX_SLOTS 2

#define RX_BLOCK_SIZE (NUM_SAMPLES*NUM_RX_SLOTS)
#define TX_BLOCK_SIZE (NUM_SAMPLES*NUM_TX_SLOTS)

/* Define a structure to represent buffers for all 8 floating-point
 * data channels of the AD21489 */
typedef struct{
	float Rx_L1[NUM_SAMPLES];
	float Rx_R1[NUM_SAMPLES];
	float Tx_L1[NUM_SAMPLES];
	float Tx_R1[NUM_SAMPLES];
} ad21479_float_data_1;

typedef struct{
	float Rx2_L1[NUM_SAMPLES];
	float Rx2_R1[NUM_SAMPLES];
	float Tx2_L1[NUM_SAMPLES];
	float Tx2_R1[NUM_SAMPLES];
} ad21479_float_data_2;

void floatData(float *output, int *input, unsigned int instep, unsigned int length);
void fixData(int *output, float *input, unsigned int outstep, unsigned int length);
void process_audioBlocks(void);
void handleCodecData(unsigned int blockIndex);
