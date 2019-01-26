/*
 * Author: Chandra Mohan C (willvalab@gmail.com)
 * All rights reserved.
 *
 * Use of this source code is governed by a MIT-style license that can be found
 * in the LICENSE file.
 *
**-----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./uartsh.h"
/*-----------------------------------------------*/

static void uartsh_puts(char const* s)
{
	while(*s)
	{
		putchar(*s++);
	}
}
/*-----------------------------------------------*/

#define HISTORY_COUNT		(UARTSH_CONFIG_COMMAND_HISTORY_COUNT + 1)

#define KEY_BS				0X08
#define KEY_DEL				0X7F
#define KEY_ESC_SEQ1		0X1B
#define KEY_SPECIAL_UP		'A'
#define KEY_SPECIAL_DOWN	'B'
#define KEY_SPECIAL_LEFT	'D'
#define KEY_SPECIAL_RIGHT	'C'
#define KEY_ESC_SEQ2		'['
#define KEY_ESC_SEQ3		'3'
#define KEY_ESC_SEQ4		'~'

#define FLAG_ESC_SEQ1		1
#define FLAG_ESC_SEQ2		2
#define FLAG_ESC_SEQ3		3

struct UartshEditBuffer
{
	int cCount;
	char buffer[UARTSH_CONFIG_COMMAND_STRING_SIZE];
};

static size_t uartsh_gets(char** command)
{
	static struct UartshEditBuffer editBuffer[HISTORY_COUNT] = { 0, };
	static int bufferIndex = 0;
	static int bufferIndexMax = 0;

	struct UartshEditBuffer* pEditBuffer = &editBuffer[bufferIndex];

	if( pEditBuffer->cCount )
	{
		if( bufferIndexMax < (HISTORY_COUNT - 1) )
			bufferIndexMax++;

		bufferIndex++;
		if( bufferIndex > bufferIndexMax )
			bufferIndex = 0;

		pEditBuffer = &editBuffer[bufferIndex];
	}

	pEditBuffer->cCount = 0;
	pEditBuffer->buffer[0] = '\0';

	size_t cursor = 0;
	char specialKey = 0;

	// 1 space for \0
	while( pEditBuffer->cCount < (UARTSH_CONFIG_COMMAND_STRING_SIZE - 1) )
	{
		char c = (char) getchar();

	#if (UARTSH_CONFIG_END_CHAR & UARTSH_CONFIG_END_CHAR_CR)
		if( c == '\r' )
		{
		#if (UARTSH_CONFIG_END_CHAR == UARTSH_CONFIG_END_CHAR_CRLF)
			c = (char) getchar();
			if( c != '\n')
				continue;
		#endif
			break;
		}
	#elif (UARTSH_CONFIG_END_CHAR & UARTSH_CONFIG_END_CHAR_LF)
		if( c == '\n' )
		{
			break;
		}
	#endif

		switch(specialKey)
		{
			case 0:
			{
				specialKey = 0;

				switch(c)
				{
					case KEY_ESC_SEQ1:
					{
						specialKey = FLAG_ESC_SEQ1;
					} break;

					case KEY_BS:
					case KEY_DEL:
					{
						if( cursor )
						{
							cursor--;
							int b = (pEditBuffer->cCount - cursor);
							memcpy(&pEditBuffer->buffer[cursor],
										&pEditBuffer->buffer[cursor + 1],
										b);
							pEditBuffer->buffer[--pEditBuffer->cCount] = '\0';

							putchar('\b');
							uartsh_puts(&pEditBuffer->buffer[cursor]);
							putchar(' ');
							while(b--)
								putchar('\b');
						}
					} break;

					default:
					{
						//only alphabets and special characters allowed
						if( (c >= 0x20) && (c <= 0x7e) )
						{
//							if( pEditBuffer->cCount < (UARTSH_CONFIG_COMMAND_STRING_SIZE - 1) )
							{
								// insertion
								int i = pEditBuffer->cCount;
								while( i > cursor )
								{
									pEditBuffer->buffer[i] = pEditBuffer->buffer[i - 1];
									i--;
								}

								pEditBuffer->buffer[cursor] = c;
								pEditBuffer->cCount++;
								pEditBuffer->buffer[pEditBuffer->cCount] = '\0';
								uartsh_puts(&pEditBuffer->buffer[cursor]);
								cursor++;

								i = (pEditBuffer->cCount - cursor);
								while(i--)
									putchar('\b');
							}
						}
					}
				}
			} break;

			case FLAG_ESC_SEQ1:
			{
				specialKey = 0;

				if( c == KEY_ESC_SEQ2 )
					specialKey = FLAG_ESC_SEQ2;
			}break;

			case FLAG_ESC_SEQ2:
			{
				specialKey = 0;

				switch(c)
				{
					case KEY_SPECIAL_UP:
					{
						while( cursor-- )
							putchar('\b');

						int oldCount = pEditBuffer->cCount;
						bufferIndex--;
						if( bufferIndex < 0 )
						{
							bufferIndex = bufferIndexMax;
						}

						pEditBuffer = &editBuffer[bufferIndex];
						uartsh_puts(pEditBuffer->buffer);
						cursor = pEditBuffer->cCount;

						if( oldCount > pEditBuffer->cCount )
						{
							int move = (oldCount - pEditBuffer->cCount);
							while( move-- )
								putchar(' ');
							move = (oldCount - pEditBuffer->cCount);
							while( move-- )
								putchar('\b');
						}
					} break;

					case KEY_SPECIAL_DOWN:
					{
						while( cursor-- )
							putchar('\b');

						int oldCount = pEditBuffer->cCount;
						bufferIndex++;
						if( bufferIndex > bufferIndexMax )
							bufferIndex = 0;

						pEditBuffer = &editBuffer[bufferIndex];

						uartsh_puts(pEditBuffer->buffer);
						cursor = pEditBuffer->cCount;

						if( oldCount > pEditBuffer->cCount )
						{
							int move = (oldCount - pEditBuffer->cCount);
							while( move-- )
								putchar(' ');
							move = (oldCount - pEditBuffer->cCount);
							while( move-- )
								putchar('\b');
						}
					} break;

					case KEY_SPECIAL_LEFT:
					{
						if( cursor )
						{
							putchar('\b');
							cursor--;
						}
					} break;

					case KEY_SPECIAL_RIGHT:
					{
						if( cursor < pEditBuffer->cCount )
						{
							putchar(pEditBuffer->buffer[cursor++]);
						}
					} break;

					case KEY_ESC_SEQ3:
					{
						specialKey = FLAG_ESC_SEQ3;
					} break;
				}
			}break;

			case FLAG_ESC_SEQ3:
			{
				specialKey = 0;

				// deletion
				if( c == KEY_ESC_SEQ4 )
				{
					if( cursor < pEditBuffer->cCount )
					{
						int b = (pEditBuffer->cCount - cursor);
						memcpy(&pEditBuffer->buffer[cursor],
									&pEditBuffer->buffer[cursor + 1],
									b);
						pEditBuffer->buffer[--pEditBuffer->cCount] = '\0';

						putchar(' ');
						putchar('\b');
						uartsh_puts(&pEditBuffer->buffer[cursor]);
						putchar(' ');
						while(b--)
							putchar('\b');
					}
				}
			}break;
		}

		fflush(stdout);
	}

	putchar('\n');

	// copy current buffer to next buffer only if current buffer has content
	if( pEditBuffer->cCount )
	{
		// as caller uses strtok passed buffer will be spoiled
		// copies current edit buffer's content to next buffer and passes to caller
		// next call makes use of passed buffer for editing
		int bi = (bufferIndex + 1);
		if( bi > (HISTORY_COUNT  - 1) )
			bi = 0;

	#if (HISTORY_COUNT >= 2) // as total buffer is 1, no point in copying to itself
		memcpy(editBuffer[bi].buffer, pEditBuffer->buffer, (pEditBuffer->cCount + 1));
	#endif

		*command = editBuffer[bi].buffer;
	}

	return pEditBuffer->cCount;
}
/*-----------------------------------------------*/

int uartshOpen( const UartshCommand commands[] )
{
	char* argv[UARTSH_CONFIG_ARGC_MAX] = { 0, };
	int argc = 0;

	while(1)
	{
		uartsh_puts("\n"UARTSH_CONFIG_PROMPT_STRING" ");
		fflush(stdout);

		char* commandString = NULL;
		int commandStringLength = 0;
		if( 0 == (commandStringLength = uartsh_gets(&commandString)) )
			continue;

		uartsh_puts(commandString);

		argc = 0;
		char* token = strtok(commandString, " \n");
		while(token != NULL)
		{
			argv[argc++] = token;
			if( argc >= UARTSH_CONFIG_ARGC_MAX )
				break;

			token = strtok(NULL, " \n");
		}

		putchar('\n');

		size_t commandLength = strlen(argv[0]);
		if( 1 == argc )
		{
			if( commandLength == 4 )
			{
				if( 0 == strncmp("help", argv[0], 4) )
				{
					puts("Supported commands are below:\n");
					int index = 0;
					while( commands[index].name != NULL )
					{
						uartsh_puts("  ");
						uartsh_puts(commands[index++].name);
						putchar('\n');
					}

					continue;
				}
				else if( 0 == strncmp("exit", argv[0], 4) )
					return 0;
			}
		}

		const UartshCommand* pUserApp = NULL;
		int index = 0;
		while( commands[index].name != NULL )
		{
			size_t appNameSize = strlen(commands[index].name);

			// extra check to ensure command length is exactly matching app length
			if( commandLength == appNameSize )
			{
				if( 0 == strncmp(argv[0], commands[index].name, appNameSize) )
				{
					pUserApp = &commands[index];
					break;
				}
			}

			index++;
		}

		if( NULL != pUserApp )
			pUserApp->handler(argc, argv);
		else
			uartsh_puts("unknown command\n");
	}

	return 0;
}
/*-----------------------------------------------*/

#if UARTSH_CONFIG_USE_ARGPARSE

int uartshParseCommand(UartshCommandParser* parser, int argc, char* argv[])
{
    struct argparse argparse;
    argparse_init(&argparse, parser->options, parser->usages, 0);
    argparse_describe(&argparse, parser->description, NULL);
	return argparse_parse(&argparse, argc, argv);
}
#endif
/*-----------------------------------------------*/

