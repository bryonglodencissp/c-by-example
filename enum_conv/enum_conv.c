/**
 * This snippets shows how weak/unsafe the type system in C is when
 *  mixing up enum parameters and integers. 
 * * Integer values are always "promoted" to enum
 * * GCC doesnt warn when assigning two different enum types (clang will warn)
 * * GCC doesnt warn when passing incompatible enum as parameter (clang will warn)
 */
#include <stdlib.h>

enum testenum {
	MYDEFINE1 = 1234
};

enum testenum2 {
	MYDEFINE2 = 8901
};

static void do_stuff(enum testenum e, size_t n)
{
	(void)e;
	(void)n;
}

int main(void)
{
	enum testenum2 e2;

	e2 = MYDEFINE1;
	do_stuff(e2, 1234);
	do_stuff(1234, 0);
	do_stuff(MYDEFINE2, 1234);

	return EXIT_SUCCESS;
}
