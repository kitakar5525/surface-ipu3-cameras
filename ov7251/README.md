### ov7251 driver, modded to use with ipu3

#### build

```bash
make
```

#### loading the module

```bash
sudo modprobe ipu3_imgu
sudo modprobe ipu3_cio2
sleep 1
sudo insmod ov7251.ko
```

#### Current state

This driver needs connection to IPU3. Currently, I'm trying to do so by overriding DSDT table.
With overriding DSDT, this driver loads without error:
```bash
kern  :warn  : [   44.204435] ov7251 i2c-INT347E:00: supply vdddo not found, using dummy regulator
kern  :warn  : [   44.204478] ov7251 i2c-INT347E:00: supply vddd not found, using dummy regulator
kern  :warn  : [   44.204489] ov7251 i2c-INT347E:00: supply vdda not found, using dummy regulator
kern  :info  : [   44.204542] ov7251 i2c-INT347E:00: Dependent platform device found: INT3472:02
kern  :info  : [   44.209302] ov7251 i2c-INT347E:00: OV7251 revision 7 (1F) detected at address 0x60
```
However, the driver doesn't get listed by libcamera:
```bash
$ sudo cam -l
[0:37:08.337418547] [7527]  INFO Camera camera_manager.cpp:283 libcamera v0.0.0+1478-1023107b
Available cameras:
```
Not sure what's missing...

#### unloading the driver

Unloading works with the following:
```bash
sudo modprobe -r ipu3_imgu
sudo modprobe -r ipu3_cio2
sudo rmmod ov7251
```

However, when I try to load the driver again, it fails to initialize:
```bash
kern  :warn  : [  109.816300] ipu3_imgu: module is from the staging directory, the quality is unknown, you have been warned.
kern  :info  : [  109.819506] ipu3-imgu 0000:00:05.0: device 0x1919 (rev: 0x1)
kern  :info  : [  109.819567] ipu3-imgu 0000:00:05.0: physical base address 0x00000000a1000000, 4194304 bytes
kern  :info  : [  109.901598] ipu3-imgu 0000:00:05.0: loaded firmware version irci_irci_ecr-master_20161208_0213_20170112_1500, 17 binaries, 1212984 bytes
kern  :info  : [  109.988233] ipu3-cio2 0000:00:14.3: device 0x9d32 (rev: 0x1)
kern  :warn  : [  111.055095] ov7251 i2c-INT347E:00: supply vdddo not found, using dummy regulator
kern  :warn  : [  111.055112] ov7251 i2c-INT347E:00: supply vddd not found, using dummy regulator
kern  :warn  : [  111.055121] ov7251 i2c-INT347E:00: supply vdda not found, using dummy regulator
kern  :info  : [  111.055167] ov7251 i2c-INT347E:00: Dependent platform device found: INT3472:02
kern  :err   : [  111.055199] ov7251 i2c-INT347E:00: cannot get enable gpio enable_gpio_0
kern  :warn  : [  111.063189] ov7251: probe of i2c-INT347E:00 failed with error -16
```

#### References

Currently, as far as I know, ov8856 is the only upstream driver that supports both DT and ACPI at the same time.
ACPI support is the first, then later added DT support by commit https://github.com/torvalds/linux/commit/0c2c7a1e0d69221b9d489bfd8cf53262d6f82446 ("media: ov8856: Add devicetree support").
