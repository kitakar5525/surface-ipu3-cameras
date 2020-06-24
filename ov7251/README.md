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
and libcamera detect the sensor, but not working saying that format is incompatible with IPU3:
```bash
[0:00:46.727329081] [2619]  INFO Camera camera_manager.cpp:283 libcamera v0.0.0+1483-c3ed943c
[0:00:46.735207399] [2620] ERROR IPU3 ipu3.cpp:1455 Sensor ov7251 8-0060 has not format compatible with the IPU3
Available cameras:
```

#### unloading the driver

```bash
sudo modprobe -r ipu3_imgu
sudo modprobe -r ipu3_cio2
sudo rmmod ov7251
```

#### References

Currently, as far as I know, ov8856 is the only upstream driver that supports both DT and ACPI at the same time.
ACPI support is the first, then later added DT support by commit https://github.com/torvalds/linux/commit/0c2c7a1e0d69221b9d489bfd8cf53262d6f82446 ("media: ov8856: Add devicetree support").
