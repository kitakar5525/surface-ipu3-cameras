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

Now, libcamera detects and lists ov7251 as "Available cameras" when using `MEDIA_BUS_FMT_SGRBG10_1X10` as code instead of `MEDIA_BUS_FMT_Y10_1X10` (which is the default of the original ov7251):
```bash
[0:01:53.858230371] [4344]  INFO Camera camera_manager.cpp:283 libcamera v0.0.0+1547-e7aa92a8
[0:01:53.869909334] [4345]  INFO IPU3 ipu3.cpp:825 Registered Camera[0] "ov5693 7-0036" connected to CSI-2 receiver 1
[0:01:53.870838496] [4345]  INFO IPU3 ipu3.cpp:825 Registered Camera[1] "ov7251 8-0060" connected to CSI-2 receiver 2
Available cameras:
1: ov5693 7-0036
2: ov7251 8-0060
```

However, it's not usable yet.
```bash
❯ sudo cam -c2 -C
[0:12:44.492671995] [6343]  INFO Camera camera_manager.cpp:283 libcamera v0.0.0+1547-e7aa92a8
[0:12:44.501870222] [6344]  INFO IPU3 ipu3.cpp:825 Registered Camera[0] "ov5693 7-0036" connected to CSI-2 receiver 1
[0:12:44.502650007] [6344]  INFO IPU3 ipu3.cpp:825 Registered Camera[1] "ov7251 8-0060" connected to CSI-2 receiver 2
Using camera ov7251 8-0060
[0:12:44.503297452] [6343]  INFO Camera camera.cpp:770 configuring streams: (0) 640x480-NV12
[0:12:44.503547214] [6344] ERROR MediaDevice media_device.cpp:802 /dev/media1[ipu3-imgu]: Failed to setup link: Invalid argument
Failed to configure camera
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
