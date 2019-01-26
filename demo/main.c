/*
 * Author: Chandra Mohan C (willvalab@gmail.com)
 * All rights reserved.
 *
 * Use of this source code is governed by a MIT-style license that can be found
 * in the LICENSE file.
 *
**-----------------------------------------------*/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "../uartsh.h"
/*-----------------------------------------------*/

int main()
{
	extern void uartshEnableRawMode();
	uartshEnableRawMode();
	extern const UartshCommand gcUartshCommands[];
	uartshOpen(gcUartshCommands);

	return 0;
}
/*-----------------------------------------------*/
