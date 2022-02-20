/*
 * initPCG.c
 *
 *  Created on: 2022年2月20日
 *      Author: EGO
 */
#include "init_PCG.h"

#include <sru.h>
#include <Cdef21489.h>

#define PCGACLKDIV 4
#define PCGAFSDIV 256

void Init_PCG(void)
{
	unsigned char n;

	*pPCG_CTLA0 = 0;

	// Wait minimum 16 cycles after disable the PCG
	for (n=0; n<16; n++)
	asm("NOP;");

	*pPCG_CTLA0 = PCGAFSDIV | ENCLKA | ENFSA; 	//pPCG_CTLA0只包含PCGAFSDIV

	*pPCG_CTLA1 = PCGACLKDIV | FSASOURCE | CLKASOURCE; //pPCG_CTLA1包含 PCGACLKDIV  /FSASOURCE / CLKASOURCE
}
