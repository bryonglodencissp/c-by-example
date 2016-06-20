reflect: reflect.o
	cc -O2 -std=c99 -g -o $@ reflect.o
reflect.o: Makefile reflect.c
	gcc -O2 -x c -std=c99 -g -c reflect.c
reflect.asm: reflect.o
	objdump -d -M intel -s reflect.o > reflect.asm
clean:
	rm -f *.o *.asm reflect
