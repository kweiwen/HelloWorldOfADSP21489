#include <sys/platform.h>
#include <iostream>
#include "init_PLL_SDRAM.h"
#include "initSRU.h"


int main(void)
{
	Init_PLL();
	Init_SDRAM();
	initDAI();

	std::cout << "Hello, World" << std::endl;

	int i = 0;
	int flag = 0;

	while(1)
	{
		i++;
		if(flag)
		{
			break;
		}
	}

	return 0;
}

