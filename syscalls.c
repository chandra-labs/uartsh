/*
 * Author: Chandra Mohan C (willvalab@gmail.com)
 * All rights reserved.
 *
 * Use of this source code is governed by a MIT-style license that can be found
 * in the LICENSE file.
 *
 **-----------------------------------------------*/

#include <sys/stat.h>
#include <sys/unistd.h>
#include <errno.h>
#include <stdio.h>
#include "./uartshConfig.h"

int _open(const char *name, int flags, int mode) {
	errno = ENOENT;
	return -1;
}

int _close(int file) {
	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		return 0;
	}
	errno = EBADF;
	return -1;
}

int _isatty(int file) {
	if (file >= STDIN_FILENO && file <= STDERR_FILENO)
		return 1;
	return 0;
}

int _read(int file, char *ptr, int len) {

	switch (file) {
		case STDIN_FILENO: /*stdin*/
		{
			char c = UARTSH_CONFIG_uart_getc();
			*ptr = c;
			return 1;
		} break;

		default:
			errno = EBADF;
	}

	return -1;
}

int _write(int file, const char *ptr, int len) {

	switch (file) {
		case STDOUT_FILENO: /*stdout*/
		case STDERR_FILENO: /*stderr*/
		{
			UARTSH_CONFIG_uart_putc(*ptr);
			return 1;
		} break;

		default:
			errno = EBADF;
	}

	return -1;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _fstat(int file, struct stat *st) {
	if (file >= STDIN_FILENO && file <= STDERR_FILENO) {
		st->st_mode = S_IFCHR;
		return 0;
	}
	errno = EBADF;
	return -1;
}

int _stat(char *file, struct stat *st) {
	errno = ENOENT;
	return -1;
}

caddr_t _sbrk(int incr) {

	static char *heap_end = UARTSH_CONFIG_HEAP_START;
	char *prev_heap_end;

	prev_heap_end = heap_end;
	if (heap_end + incr > UARTSH_CONFIG_HEAP_END) {
		printf("Heap full (requested %d, available %d)\n", incr,
				(int) (UARTSH_CONFIG_HEAP_END - heap_end));
		errno = ENOMEM;
		return (caddr_t) - 1;
	}

	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

int _fsync(int fd) {
	if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
		return 0;
	}
	if (fd == STDIN_FILENO)
		return 0;
	errno = EBADF;
	return -1;
}

void _exit(int status) {
	while (1);
}

void _abort() {
	while (1);
}

void _kill() {
	while (1);
}

pid_t _getpid(void) {
	return 1;
}
