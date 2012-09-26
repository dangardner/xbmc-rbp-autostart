/*
 * Virtual terminal helper program to display a prompt to the user to press the
 * Esc key within the specified timeout, in order to access a getty (the
 * default timeout is 10 seconds).
 *
 * Exits with status 8 to indicate Esc was pressed or 9 to indicate a timeout.
 *
 * Author: Dan Gardner <dan+rpi@chaotica.org.uk>
 * License: GPLv3
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define DEFAULT_TIMEOUT	10

#define EXIT_KEYPRESS	8
#define EXIT_TIMEOUT	9

static struct termios oldt, newt;

void unbuffer_stdin()
{
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restore_stdin()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	printf("\n");
}

void timeout_handler()
{
	printf("timed out.");
	restore_stdin();
	exit(EXIT_TIMEOUT);
}

int main(int argc, char **argv)
{
	int timeout = DEFAULT_TIMEOUT;
	struct sigaction sact;

	if (argc >= 2 && sscanf(argv[1], "%d", &timeout) != 1)
	{
		fprintf(stderr, "Syntax: %s [timeout seconds]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	printf("Press the \"Esc\" key within %d seconds to access a terminal...", timeout);

	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	sact.sa_handler = timeout_handler;
	sigaction(SIGALRM, &sact, NULL);

	alarm(timeout);

	unbuffer_stdin();
	while (1)
	{
		if (getchar() != 27)
			continue;
		restore_stdin();
		exit(EXIT_KEYPRESS);
	}
	/* should never reach here */
	restore_stdin();
	return 0;
}
