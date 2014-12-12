all:
	cc -E a.c -o a.pp
	cc -o a a.c
	ln -fs a b
	ln -fs a c

clean:
	rm a a.pp b c
