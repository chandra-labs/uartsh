/*
 * Author: Chandra Mohan C (willvalab@gmail.com)
 * All rights reserved.
 *
 * Use of this source code is governed by a MIT-style license that can be found
 * in the LICENSE file.
 *
**-----------------------------------------------*/

#ifndef UARTSH_H
#define UARTSH_H
/*-----------------------------------------------*/

#include "./argparse.h"
#include "./uartshConfig.h"
/*-----------------------------------------------*/

#define UARTSH_OPTION_BEGIN()				OPT_HELP()
#define UARTSH_OPTION_END()                 OPT_END()
#define UARTSH_OPTION_BOOLEAN(...)     		OPT_BOOLEAN(__VA_ARGS__)
#define UARTSH_OPTION_BIT(...)   		    OPT_BIT(__VA_ARGS__)
#define UARTSH_OPTION_INTEGER(...) 		    OPT_INTEGER(__VA_ARGS__)
#define UARTSH_OPTION_FLOAT(...)       		OPT_FLOAT(__VA_ARGS__)
#define UARTSH_OPTION_STRING(...)      		OPT_STRING(__VA_ARGS__)
#define UARTSH_OPTION_GROUP(h)              OPT_GROUP(h)
/*-----------------------------------------------*/

#define UARTSH_INCLUDE_CMD(handler) extern int handler(int argc, char* argv[])
#define UARTSH_REGISTER_CMD(name, handler)	{ name, handler }
typedef struct argparse_option UartshCommandOption;
/*-----------------------------------------------*/

typedef struct UartshCommandParser
{
	const char** usages;
	const char* description;
	UartshCommandOption* options;
} UartshCommandParser;
/*-----------------------------------------------*/

typedef struct UartshCommand
{
	char const* name;
	int (*handler)(int argc, char* argv[]);
} UartshCommand;
/*-----------------------------------------------*/

size_t uartsh_gets(char* buffer, size_t size);
size_t uartsh_puts(char* buffer, size_t size);
int uartshOpen(const UartshCommand commands[]);
int uartshParseCommand(UartshCommandParser* parser, int argc, char* argv[]);
/*-----------------------------------------------*/
#endif /* UARTSH_H */
