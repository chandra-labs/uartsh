/*
 * Author: Chandra Mohan C (willvalab@gmail.com)
 * All rights reserved.
 *
 * Use of this source code is governed by a MIT-style license that can be found
 * in the LICENSE file.
 *
**-----------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "./uartsh.h"
/*-----------------------------------------------*/

size_t uartsh_puts(char* buffer, size_t size)
{
	int count = size;
	while(count--)
	{
		putchar(*buffer++);
	}

	return size;
}
/*-----------------------------------------------*/

#define KEY_BS				0X08
#define KEY_DEL				0X7F
#define KEY_ESC_SEQ1		0X1B
#define KEY_SPECIAL_LEFT	'D'
#define KEY_SPECIAL_RIGHT	'C'
#define KEY_ESC_SEQ2		'['
#define KEY_ESC_SEQ3		'3'
#define KEY_ESC_SEQ4		'~'

#define FLAG_ESC_SEQ1		1
#define FLAG_ESC_SEQ2		2
#define FLAG_ESC_SEQ3		3

size_t uartsh_gets(char buffer[], size_t size)
{
	size_t cursor = 0;
	char specialKey = 0;
	size_t cCount = 0;
	char c = 0;

	if( size > UARTSH_CONFIG_COMMAND_STRING_SIZE )
		size = UARTSH_CONFIG_COMMAND_STRING_SIZE;

	// reserve space for \0
	size--;

	buffer[cCount] = '\0';

	while( cCount < size )
	{
		c = (char) getchar();

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
							int b = (cCount - cursor);
							memcpy(&buffer[cursor],
										&buffer[cursor + 1],
										b);
							buffer[--cCount] = '\0';

							printf("\b%s ", &buffer[cursor]);
							while(b--)
								putchar('\b');
						}
					} break;

					default:
					{
						//only alphabets and special characters allowed
						if( (c >= 0x20) && (c <= 0x7e) )
						{
							if( cCount < size )
							{
								int count = ++cCount;

								while(count > cursor)
								{
									buffer[count] = buffer[count - 1];
									count--;
								}

								buffer[cursor] = c;

								printf("%s", &buffer[cursor]);
								cursor++;

								count = cCount - cursor;
								while(count--)
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
						if( cursor < cCount )
						{
							putchar(buffer[cursor++]);
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

				if( c == KEY_ESC_SEQ4 )
				{
					if( cursor < cCount )
					{
						int b = (cCount - cursor);
						memcpy(&buffer[cursor],
									&buffer[cursor + 1],
									b);
						buffer[--cCount] = '\0';

						printf(" \b%s \b", &buffer[cursor]);
						while(--b)
							putchar('\b');
					}
				}
			}break;
		}

		fflush(stdout);
	}

	putchar('\n');

	return cCount;
}
/*-----------------------------------------------*/

int uartshOpen( const UartshCommand commands[] )
{
	char buffer[UARTSH_CONFIG_COMMAND_STRING_SIZE] = { 0, };
	char* argv[UARTSH_CONFIG_ARGC_MAX + 1] = { 0, };
	int argc = 0;

	while(1)
	{
		printf("\n"UARTSH_CONFIG_PROMPT_STRING" ");
		fflush(stdout);

#if 0
		char* pBuffer = buffer;
		size_t bufferSize = UARTSH_CONFIG_COMMAND_STRING_SIZE;
		if( 0 == getline(&pBuffer, &bufferSize, stdin) )
			continue;
#else
		if( 0 == uartsh_gets(buffer, sizeof(buffer)) )
			continue;
#endif

		printf("%s\n", buffer);

		argc = 0;
		char* token = strtok(buffer, " \n");
		while(token != NULL)
		{
			argv[argc++] = token;
			token = strtok(NULL, " \n");
		}

		putchar('\n');

		size_t commandLength = strlen(argv[0]);
		if( 1 == argc )
		{
			if( (commandLength == 2) && (0 == strncmp("ls", argv[0], 2)) )
			{
				puts("Supported commands are below:\n");
				int index = 0;
				while( commands[index].name != NULL )
				{
					printf("  %s\n", commands[index++].name);
				}

				continue;
			}
			else if( (commandLength == 4) && (0 == strncmp("exit", argv[0], 4)) )
				return 0;
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
			puts("unknown command");
	}

	return 0;
}
/*-----------------------------------------------*/

int uartshParseCommand(UartshCommandParser* parser, int argc, char* argv[])
{
    struct argparse argparse;
    argparse_init(&argparse, parser->options, parser->usages, 0);
    argparse_describe(&argparse, parser->description, NULL);
	return argparse_parse(&argparse, argc, argv);
}
/*-----------------------------------------------*/
