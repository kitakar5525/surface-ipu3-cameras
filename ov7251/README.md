### ov7251 driver, modded to use with ipu3

#### build

```bash
make
```

#### loading the module

```bash
sudo insmod ov7251.ko
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
