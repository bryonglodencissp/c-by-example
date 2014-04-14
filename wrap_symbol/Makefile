CC=$(CROSS_COMPILE)gcc

all: wrap_symbol.c
	$(CC) -c wrap_symbol.c
	$(CC) -Wl,-wrap,malloc,-wrap,free,-wrap,calloc,-wrap,realloc -o wrap_symbol wrap_symbol.o

clean:
	rm -f *.o
	rm wrap_symbol
