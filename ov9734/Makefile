MODULE_NAME := "ov9734"

KVERSION := "$(shell uname -r)"

obj-m += ov9734.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
