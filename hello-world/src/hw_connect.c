/*
 * hw_connect.c
 *
 *  Created on: 2022��2��20��
 *      Author: EGO
 */

#include <sru.h>
#include "hw_connect.h"

void Run_LED_Blink(void)
{
	static Flag = 0;
	if(Flag){
		_Run_LED_On();
		Flag = 0;
	}else{
		_Run_LED_Off();
		Flag = 1;
	}
}
