/*
 * uartshCommands.c
 *
 *  Created on: Feb 2, 2019
 *      Author: cm943953
 */
/*-----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "./uartsh.h"
/*-----------------------------------------------*/

int uartshCommand_mr(int argc, char* argv[])
{
	char const* const usage[] =
	{
			"mr -u32 <address> <count>",
			"mr -u32 <address> --bytes 100",
			NULL,
	};

	int flags = 0;
	int width = 8;
	char* address = NULL;
	int count = 1;

#define MR_BYTES	(1 << 0)

	UartshCommandOption const options[] =
	{
			UARTSH_OPTION_BEGIN(),
			UARTSH_OPTION_GROUP("memory read options"),
			UARTSH_OPTION_INTEGER('u', NULL, &width, "data width [8|16|32|64]"),
			UARTSH_OPTION_BIT('b', "bytes", &flags, "n bytes to read", NULL, MR_BYTES),
			UARTSH_OPTION_END(),
	};

	UartshCommandParser parser = {
			.usages = (char const**) usage,
			.description = "Memory read operations",
			.options = (UartshCommandOption*) options,
	};

	argc = uartshParseCommand(&parser, argc, argv);

	if( argc > 0 )
	{
		if( argc > 0 )
		{
			address = (char*) strtoull(argv[0], NULL, 16);
			if( NULL == address )
			{
				puts("address field missing");
				return -1;
			}
		}

		if( argc > 1 )
			count = (int) strtoul(argv[1], NULL, 0);

		width >>= 3;
		unsigned long bytes = count;

		if( 0 == (flags & MR_BYTES) )
			bytes = (count * width);

		unsigned long balance = bytes;

		for( int i = 0; i < bytes; i += width )
		{
			if( 0 == (i & 0xf) )
				printf("\n%p:", (address + i));

			if( balance < width )
			{
				if( balance < 2 )
					width = 1;
				else if( balance < 4 )
					width = 2;
				else if( balance < 8 )
					width = 4;
			}

			switch( width )
			{
				case 1:
				{
					printf(" %02x", (unsigned char)(address[i] & 0xff));
				} break;

				case 2:
				{
					printf(" %04x", *(unsigned short*)&address[i]);
				} break;

				case 4:
				{
					printf(" %08x", *(unsigned int*)&address[i]);
				} break;

				case 8:
				{
					printf(" %016llx", *(unsigned long long*)&address[i]);
				} break;
			}

			balance -= width;
		}

		printf("\n%ld bytes read\n", bytes);
	}

	return 0;
}
/*-----------------------------------------------*/

int uartshCommand_mw(int argc, char* argv[])
{
	char const* const usage[] =
	{
			"mw -u32 <address> <data> <count>",
			NULL,
	};

	int inc = 0;
	int width = 8;
	char* address = NULL;
	unsigned long long data = 0;
	int count = 1;

	UartshCommandOption const options[] =
	{
			UARTSH_OPTION_BEGIN(),
			UARTSH_OPTION_GROUP("memory write options"),
			UARTSH_OPTION_INTEGER('u', NULL, &width, "data width [8|16|32|64]"),
			UARTSH_OPTION_BOOLEAN('I', "inc", &inc, "incremental writes"),
			UARTSH_OPTION_END(),
	};

	UartshCommandParser parser = {
			.usages = (char const**) usage,
			.description = "Memory write operations",
			.options = (UartshCommandOption*) options,
	};

	argc = uartshParseCommand(&parser, argc, argv);

	if( argc > 0 )
	{
		if( argc > 0 )
		{
			address = (char*) strtoull(argv[0], NULL, 16);
			if( NULL == address )
			{
				puts("address field missing");
				return -1;
			}
		}

		if( argc > 1 )
			data = (unsigned long long) strtoull(argv[1], NULL, 0);

		if( argc > 2 )
			count = (int) strtoul(argv[2], NULL, 0);

		width >>= 3;
		unsigned long bytes = (count * width);

		for( int i = 0; i < bytes; i += width )
		{
			switch( width )
			{
				case 1:
				{
					address[i] = (unsigned char) data;
				} break;

				case 2:
				{
					*(unsigned short*)&address[i] = (unsigned short) data;
				} break;

				case 4:
				{
					*(unsigned int*)&address[i] = (unsigned int) data;
				} break;

				case 8:
				{
					*(unsigned long long*)&address[i] = (unsigned long long) data;
				} break;
			}

			if(inc)
				data++;
		}

		printf("\n%ld bytes written\n", bytes);
	}

	return 0;
}
/*-----------------------------------------------*/

