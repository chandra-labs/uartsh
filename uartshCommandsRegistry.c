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

#if UARTSH_CONFIG_USE_ARGPARSE
UARTSH_INCLUDE_CMD(uartshArgparse_read);
#endif
/*-----------------------------------------------*/

// ------------- register user commands
const UartshCommand gcUartshCommands[] = {

#if UARTSH_CONFIG_USE_ARGPARSE
		UARTSH_REGISTER_CMD("memread", uartshArgparse_read),
#endif

		// last must end with null
		UARTSH_REGISTER_CMD(NULL, NULL),
};
/*-----------------------------------------------*/
