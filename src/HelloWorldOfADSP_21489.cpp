/*****************************************************************************
 * HelloWorldOfADSP_21489.cpp
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "HelloWorldOfADSP_21489.h"
#include <iostream>

char __argv_string[] = "";

int main(int argc, char *argv[])
{
	adi_initComponents();
	
	std::cout << "Hello, World!" << std::endl;
	std::cout << "Hello, World!" << std::endl;

	/* Begin adding your custom code here */

	return 0;
}

