MODULE_NAME := "ov8865"

KVERSION := "$(shell uname -r)"

obj-m += ov8865.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
