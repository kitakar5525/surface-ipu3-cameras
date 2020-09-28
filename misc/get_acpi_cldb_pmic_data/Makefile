KVERSION := "$(shell uname -r)"

obj-m += tps68470_dump_cldb.o

all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
