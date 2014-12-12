# https://gist.github.com/llj098/752417#file-tcp_svr_sample-c
obj-m += tcp_svr_sample.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
