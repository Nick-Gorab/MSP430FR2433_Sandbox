/*
 * led.c
 *
 *  Created on: May 9, 2020
 *      Author: Nick
 */
#include "system.h"

int initAllLEDs(void) {
	P1DIR |= 0x03;	// Initializes both P1.0 and P1.1
	return 0;
}




int initMultiLEDs (int pin_number, ...) {
	int num = 0;
	int i = 0;
	va_list arguments;
	va_start(arguments, pin_number);

	for (i = 0; i < pin_number; i++) {
		num += va_arg(arguments, int);
	}
	va_end(arguments);
	return num;
}

