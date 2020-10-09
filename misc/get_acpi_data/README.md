#### build
```bash
make
```

#### loading module
```bash
# make sure that sensor drivers are not loaded
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

sudo insmod get_acpi_data.ko
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
