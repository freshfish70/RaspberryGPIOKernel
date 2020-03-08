CFLAGS=-std=c99
obj-m += gpiodriver.o
gpio_module-obj:= hgpio.o gpiodriver.o

all:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean