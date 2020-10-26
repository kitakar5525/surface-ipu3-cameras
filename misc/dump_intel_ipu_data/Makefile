KVERSION := "$(shell uname -r)"

obj-m += dump_intel_ipu_data.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
