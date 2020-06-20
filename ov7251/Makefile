MODULE_NAME := "ov7251"

KVERSION := "$(shell uname -r)"

obj-m += ov7251.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
