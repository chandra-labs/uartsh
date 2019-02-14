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

UARTSH_INCLUDE_CMD(uartshCommand_mr);
UARTSH_INCLUDE_CMD(uartshCommand_mw);
/*-----------------------------------------------*/

// ------------- register user commands
const UartshCommand gcUartshCommands[] = {

		UARTSH_REGISTER_CMD("mr", uartshCommand_mr),
		UARTSH_REGISTER_CMD("mw", uartshCommand_mw),

		// last must end with null
		UARTSH_REGISTER_CMD(NULL, NULL),
};
/*-----------------------------------------------*/
