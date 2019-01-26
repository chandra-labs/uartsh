/*
 * rawConsole.c
 *
 *  Created on: 26-Jan-2019
 *      Author: chandra
 */

#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
/*-----------------------------------------------*/

static struct termios orig_termios;

static void die(const char *s)
{
	perror(s);
	exit(1);
}
/*-----------------------------------------------*/

static void disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
	{
		die("tcsetattr");
	}
}
/*-----------------------------------------------*/

void uartshEnableRawMode()
{
	if(tcgetattr(STDIN_FILENO, &orig_termios) == -1)
	{
		die("tcgetattr");
	}

	atexit(disableRawMode);
	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	//  raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN /*| ISIG*/);

	//raw.c_cc[VMIN] = 0;
	//raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
	{
		die("tcsetattr");
	}
}
/*-----------------------------------------------*/

