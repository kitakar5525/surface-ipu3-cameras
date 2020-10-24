# SPDX-License-Identifier: GPL-2.0
MODULE_NAME := "ov569x"

KVERSION := "$(shell uname -r)"

obj-m += ov569x.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
