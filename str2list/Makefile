CC=$(CROSS_COMPILE)gcc

str2list:
	$(CC) -Wall -Wextra -ggdb -O0 -o $@ $@.c

test: str2list
	./str2list "(0)(0,4)(0,4,8)"

clean:
	rm -f *.o
	rm str2list
