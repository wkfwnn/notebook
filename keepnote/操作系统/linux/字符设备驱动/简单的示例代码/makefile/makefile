obj-m :=1.o  
LINUX_KERNEL:=$(shell uname -r)  
#the absolute path  
LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(LINUX_KERNEL)
PWD :=$(shell pwd)  
modules :  
	$(MAKE) -C $(LINUX_KERNEL_PATH) M=$(PWD) modules  
.PHONEY:clean  
clean :  
	rm -f *.o *.ko  