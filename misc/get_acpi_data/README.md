#### build
```bash
make
```

#### loading module and getting data from dmesg
```bash
# Print your system info:
# - do not use sudo so that it doesn't contain personal data
# - ignoring errors
grep . /sys/class/dmi/id/* 2>/dev/null

# clear the kernel ring buffer so that dmesg doesn't contain unnecessary log
sudo dmesg -C

# load/unload module then output dmesg log
sudo insmod get_acpi_data.ko
sleep 1
sudo rmmod get_acpi_data
sudo dmesg
```

Note that to print i2c device name with the current bridge driver (v2
version), you still need to unload the real sensor drivers before loading
this module (https://github.com/kitakar5525/surface-ipu3-cameras/commit/582c4ba3bba5812024f5efd33769c6eafd50f371#diff-7e53e40ecaeff527b241d9f9eaec4af11778953028b4bf510d61c9d34c3b4e70R394-R401):
```bash
# unload the real sensor drivers
sudo modprobe -r ipu3_imgu
sudo modprobe -r ipu3_cio2_driver
sudo modprobe -r atomisp-ov5693
sudo modprobe -r ov5693
sudo modprobe -r ov5670
sudo modprobe -r ov8865
sudo modprobe -r ov7251
sudo modprobe -r ov2680
sudo modprobe -r ov5648
sudo modprobe -r ov9734
sudo modprobe ipu3_cio2_driver
sudo modprobe ipu3_imgu
```

#### References
- https://github.com/jhand2/surface-camera/tree/8c660116193c2e8acb87b964366bd819f2a68188/tools
  [ssdb_dump]
- https://github.com/coreboot/coreboot/blob/6d9f24383539bcd2d24a31054a41977e5d004c5e/src/drivers/intel/mipi_camera/chip.h#L135-L188
  [intel_ssdb]
- https://chromium-review.googlesource.com/c/chromiumos/third_party/kernel/+/709778/5/drivers/media/pci/intel/ipu3/intel-acpi-camera.c#b65
  [CLDB structure]
- https://chromium-review.googlesource.com/c/chromiumos/third_party/kernel/+/709778/5/include/media/intel-acpi-camera.h#b43
  [comment for control_logic_type]

and based on my previous works:
- https://github.com/kitakar5525/surface-ipu3-cameras/tree/823ef203a966a08993286863df76f2ef24aacd81/misc/get_acpi_ssdb_sensor_data
- https://github.com/kitakar5525/surface-ipu3-cameras/tree/23f7f37bedc556de163f51a66e8edae97054241b/misc/ssdb_dump_from_jhand2
