KVERSION := "$(shell uname -r)"

obj-m += ov5693.o
obj-m += ov8865.o
obj-m += ov7251.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
