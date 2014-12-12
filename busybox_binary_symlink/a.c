#include <stdio.h>
#include <libgen.h>

void test_print_args(int argc, char **argv)
{
	int i = 0;
	printf("\targc: %d\n", argc);
	for(i = 0; i < argc; i++)
		printf("\targ[%u]: %s\n", i, argv[i]);
}

int a(int argc, char **argv)
{
	printf("This is the body of test %s\n{\n", __func__);
	test_print_args(argc, argv);
	printf("}\n");
}

int b(int argc, char **argv)
{
	printf("This is the body of test %s\n{\n", __func__);
	test_print_args(argc, argv);
	printf("}\n");
}

int c(int argc, char **argv)
{
	printf("This is the body of test %s\n{\n", __func__);
	test_print_args(argc, argv);
	printf("}\n");
}

struct test {
	const char *name;
	const char *description;
	int (*function)(int argc, char **argv);
} test_config[] = {
#define TEST(name, desc) #name, desc, name,
TEST(a, "Test for the a function")
TEST(b, "Test for the b function")
TEST(c, "Test for the c function")
#undef TEST
	NULL
};

int (*test_get(char *name))(int argc, char **argv)
{
	int i;

	for (i = 0; i < sizeof(test_config) / sizeof(test_config[0]); i++) {
		if(test_config[i].name != NULL) {
			if(strcmp(name, test_config[i].name) == 0)
				return test_config[i].function;
		}
	}

	return NULL;
}

void test_available(void)
{
	unsigned int i = 0;
	struct test *test_current = &test_config[0];

	printf("Tests available:\n");

	do {
		printf("\t[%u] %s: %s\n", i,
			test_current->name,
			test_current->description);
		i++;
		test_current = &test_config[i];
	} while(test_current->name != NULL);
}

int test_run(char *name, int argc, char **argv)
{
	int (*test)() = NULL;

	argc--;
	argv = &argv[1];

	/* Run test from argument */
	test = test_get(name);
	if (test != NULL) {
		(*test)(argc, argv);
	} else {
		printf("Test %s not available!!!\n", name);
		test_available();
	}
}

int main(int argc, char **argv, char **env)
{
	char *arg = NULL;
	unsigned int i = 0;

	arg = basename(argv[0]);
	printf("argv[%u]: basename(%s) = %s\n", i, argv[0], arg);
	test_run(arg, argc, argv);

	return 0;
}
