/*
 * Author: Chandra Mohan C (willvalab@gmail.com)
 * All rights reserved.
 *
 * Use of this source code is governed by a MIT-style license that can be found
 * in the LICENSE file.
 *
**-----------------------------------------------*/

#include <stdio.h>
#include "./uartsh.h"
/*-----------------------------------------------*/

int main()
{
	extern const UartshCommand gcUartshCommands[];
	uartshOpen(gcUartshCommands);
	
	return 0;
}
/*-----------------------------------------------*/
