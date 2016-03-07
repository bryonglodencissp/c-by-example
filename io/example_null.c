#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "io.h"

xg_ssize_t read_stdin(uint8_t *dst, size_t n)
{
	ssize_t _n = fread(dst, n, 1, stdin);
	if (_n >= 0)
		return _n;

	return -1;
}

int main(void)
{
	struct termios old, new;

	struct xg_io_reader r = { .read = read_stdin };
	struct xg_io_writer w = { .write = NULL };

	if (isatty(STDIN_FILENO) == 1) {
		/* Turn echoing off and fail if we can't. */
		if (tcgetattr (STDIN_FILENO, &old) != 0)
			return -1;
		new = old;
		new.c_lflag &= ~ECHO;
		if (tcsetattr (STDIN_FILENO, TCSAFLUSH, &new) != 0)
			return -1;
	}

	/* copy stdin to stdout */
	xg_io_copy(&w, &r);

	if (isatty(STDIN_FILENO) == 1) {
		/* Restore terminal. */
		(void) tcsetattr (STDIN_FILENO, TCSAFLUSH, &old);	
	}

	return 0;
}
