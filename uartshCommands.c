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

int uartshArgparse_read(int argc, char* argv[])
{
		static char const* usages[] =
		{
			"memread -a <address> -c <count>",
			"memread -a 0x10000000 -c 64",
			"memread --address <address> --count <count>",
			NULL,
		};

		unsigned int address = 0;
		unsigned int dwords = 0;

		// you can get more information on this from argparse repo [https://github.com/cofyc/argparse]
		UartshCommandOption options[] = {
			UARTSH_OPTION_BEGIN(),
			UARTSH_OPTION_GROUP("memory read options"),
			UARTSH_OPTION_INTEGER('a', "address", &address, "memory address to read"),
			UARTSH_OPTION_INTEGER('c', "count", &dwords, "number of bytes to read"),
			UARTSH_OPTION_END(),
		};

		UartshCommandParser parser = {
				.usages = usages,
				.description = "Memory read / write operations",
				.options = options,
		};

		argc = uartshParseCommand(&parser, argc, argv);

		if( argc >= 0 )
		{
				volatile unsigned int* memory = (unsigned int*) (address & ~0b11);

				for(int i = 0; i < dwords; i++)
				{
						if( 0 == (i & 0b11) )
							printf("\n0x%08x:", (unsigned int) memory);

						printf(" 0x%08x", (unsigned int) *memory++);
				}
		}

		return 0; // return values are not handled and of no use
}
#endif
/*-----------------------------------------------*/

