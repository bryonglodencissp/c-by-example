#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static int _signum;
static bool _signal_unhandled = false;

void
sighandler(int signum)
{
	fprintf(stderr, "Caught signal %d\n",signum);
	fflush(stderr);

	_signum = signum;
	_signal_unhandled = true;

	if(signum == SIGBUS) {
		signal(signum, SIG_IGN);
		signal(signum, sighandler);
	}
}

int main()
{
	signal(SIGBUS, sighandler);

	while(1) {
		printf("Program processing stuff here.\n");
		sleep(1);

		if (_signal_unhandled) {
			switch(_signum) {
			case SIGBUS:
				printf("We catched a bus error\n");
			break;
			default:
				break;
			}
			_signal_unhandled = false;
		}
	}

	return EXIT_SUCCESS;
}
