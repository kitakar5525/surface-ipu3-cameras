MODULE_NAME := "ov5693"

KVERSION := "$(shell uname -r)"

obj-m += ov5693.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
