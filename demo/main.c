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
	char* buffer = malloc((1024 * 4) + 7);
	buffer = (char*) ( ((size_t)buffer) & ~((size_t)7) );

	printf("%d bytes of buffer %p for r/w operations\n", (1024 * 4), buffer);

	extern void uartshEnableRawMode();
	uartshEnableRawMode();
	extern const UartshCommand gcUartshCommands[];
	uartshOpen("uartsh# ", gcUartshCommands);

	free(buffer);

	return 0;
}
/*-----------------------------------------------*/
