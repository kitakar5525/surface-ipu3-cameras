KVERSION := "$(shell uname -r)"
KDIR := "/lib/modules/$(KVERSION)/build"

obj-m += drivers/media/i2c/ov5693.o
obj-m += drivers/media/i2c/ov7251.o
obj-m += drivers/media/i2c/ov8865.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
