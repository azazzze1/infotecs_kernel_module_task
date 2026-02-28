obj-m += logModule.o
logModule-objs := logModuleMain.o logModuleAttributes.o logModuleLogic.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

load:
	sudo insmod logModule.ko

unload:
	sudo rmmod logModule

.PHONY: all clean load unload