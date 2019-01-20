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

#include "./uartsh.h"
/*-----------------------------------------------*/

struct termios orig_termios;

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
//  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN /*| ISIG*/);

  //raw.c_cc[VMIN] = 0;
  //raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int main()
{
	enableRawMode();
//	setvbuf(stdin, NULL, _IONBF, 0);
#if 0
	while(1)
	{
		char c = getchar();

		if( (c >= 0x20) && (c <= 0x7e) )
		{
			putchar(c);
		}
		else if( (c == '\r') || (c == '\n') )
		{
			putchar('\r');
			putchar('\n');
		}
		else
		{
			printf("_%02x_", c);
		}

		fflush(stdout);
	}
#else
	extern const UartshCommand gcUartshCommands[];
	uartshOpen(gcUartshCommands);
#endif

	return 0;
}

/*-----------------------------------------------*/
