/*
 * hw_connect.h
 *
 *  Created on: 2022Äê2ÔÂ14ÈÕ
 *      Author: manchiu
 */
#include <sru.h>

#ifndef HW_CONNECT_H_
#define HW_CONNECT_H_


#pragma once
#ifdef __cplusplus
extern "C" {
#endif

	#define _Run_LED_On()		SRU2(HIGH, DPI_PB14_I)
	#define _Run_LED_Off()		SRU2(LOW, DPI_PB14_I)


	#define _Codec_Reset_H()	SRU2(HIGH, DPI_PB13_I)
	#define _Codec_Reset_L()	SRU2(LOW, DPI_PB13_I)

	void Run_LED_Blink(void);
	#endif /* HW_CONNECT_H_ */


#ifdef __cplusplus
}
#endif
