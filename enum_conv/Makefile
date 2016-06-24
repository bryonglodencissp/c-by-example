enum_conv: enum_conv.c Makefile
	clang-3.8 -Wall -Wextra -Wduplicate-enum -o enum_conv.clang-3.8 enum_conv.c
	gcc -Wall -Wextra -o enum_conv enum_conv.c

clean:
	rm -f *.o
	rm enum_conv
