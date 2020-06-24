MODULE_NAME := "surface_camera"

KVERSION := "$(shell uname -r)"

obj-m += surface_camera.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
