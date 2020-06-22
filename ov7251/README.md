### ov7251 driver, modded to use with ipu3

#### build

```bash
make
```

#### loading the module

```bash
sudo modprobe -r ov7251 # unload if already loaded
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

#### can't unload driver

```bash
$ sudo rmmod ov7251
rmmod: ERROR: Module ov7251 is in use
```

#### References

Currently, as far as I know, ov8856 is the only upstream driver that supports both DT and ACPI at the same time.
ACPI support is the first, then later added DT support by commit https://github.com/torvalds/linux/commit/0c2c7a1e0d69221b9d489bfd8cf53262d6f82446 ("media: ov8856: Add devicetree support").
