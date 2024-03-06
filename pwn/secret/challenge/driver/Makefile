obj-m := secret.o
KBUILD_DIR := /home/ptr/app/buildroot/output/build/linux-6.1.12

all:
	$(MAKE) -C $(KBUILD_DIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KBUILD_DIR) M=$(shell pwd) clean
