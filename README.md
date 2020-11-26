#### Trying to get IPU3 cameras working on Surface devices

Link to issue: https://github.com/linux-surface/linux-surface/issues/91

References:
- https://github.com/jhand2/surface-camera
  [I referenced GPIO part.]

#### atomisp/IPU3/IPU4

- Surface 3 uses atomisp (sometimes also called as IPU2)
- Surface gen 4 to 6 devices use IPU3.
- Surface gen 7 devices use IPU4 instead (except SL3, which uses USB cameras instead).

For S3 cameras, I made a separate repo: https://github.com/kitakar5525/surface3-atomisp-cameras

#### What device uses what sensors?

From linux-surface's acpidumps repo and Surface firmware download files:

```bash
# S3 (atomisp)
CAM6 APTA0330 AR0330 # Front cam
CAM1 OVTI8835 OV8835 # Rear cam

# SP/SB/SGO with gen4+ (SP4 and later gen) devices
CAMF INT33BE OV5693
CAMR INT347A OV8865
CAM3 INT347E OV7251 # IR cam

# Note that SP7/SB3 uses ipu4. ipu4 driver isn't upstream and currently
# available up to only 4.19, someone need to port it to newer kernel
# series. Take a look at links section below for more info.

# SL1/SL2 (SL3 uses USB cameras instead)
CAMF OVTI9734 OV9734
CAM3 INT347E OV7251 # IR cam

# non-Surface devices
# Acer Switch Alpha 12
# Front cam is USB
LNK0 INT3479 OV5670 # Rear cam
```

#### usage

First, build kernel with the following changes:

0. Set the kernel config `CONFIG_INIT_STACK_NONE=y` or `CONFIG_GCC_PLUGIN_STRUCTLEAK_USER=y` (https://github.com/kitakar5525/surface-ipu3-cameras/issues/5) to avoid kernel crash.
   Another way is to apply patches currently under development by upstream kernel devs: [[v3,0/8] ipu3-cio2 format and other fixes](https://patchwork.kernel.org/project/linux-media/cover/20201012180414.11579-1-sakari.ailus@linux.intel.com/)
1. Apply software node kernel patch and add bridge driver

   Both software node and bridge driver patches are available in djrscally's repo: https://github.com/djrscally/miix-510-cameras/tree/master/patches
2. For the SGO series owners: the SGO series need `acpi_enforce_resources=lax` kernel parameter for the front cam (ov5693) to work.
   Add it to your bootloader.

Then, build sensor drivers:

- Go to sensor drivers dir (e.g., ov5693) and follow the README there

To try to capture images, you can use libcamera:
```bash
# If you use Arch Linux, libcamera-git is available in AUR. If not
# available on your distro, build it yourself:

# clone libcamera
git clone git://linuxtv.org/libcamera.git
cd libcamera
# Dependencies are documented in README.rst file.
# build libcamera with v4l2 compat layer library
meson build -Dv4l2=true --reconfigure
ninja -C build

# to access /dev/media? without sudo, add user to video group:
sudo gpasswd -a ${USER} video
# then logout and login again for it to take effect

# list available cameras:
cam -l

# try to capture images:
qcam
```

Note:

- About the `dsdt-mods` dir: DSDT overriding was needed before because the bridge driver didn't exist then. Now that the bridge driver is stable enough, it's not needed anymore. I'll leave it there for reference.

#### how to reload camera drivers?
Sensor drivers are used by ipu3 drivers. So, we need to unload ipu3 drivers first like this:
```bash
# reload camera drivers
sudo modprobe -r ipu3_imgu
sudo modprobe -r ipu3_cio2
sudo rmmod ov5693
sudo rmmod ov7251
sudo rmmod ov8865

sudo modprobe ipu3_cio2
sudo modprobe ipu3_imgu
sudo modprobe ov5693
sudo modprobe ov7251
sudo modprobe ov8865
```

---

#### links

Intel maintains IPU4 (not IPU3) drivers (crlmodule, ipu4-acpi, and ipu4) at linux-intel-lts repo (up to v4.19):
- https://github.com/intel/linux-intel-lts/tree/4.19/base

Especially, we may be interested in the crlmodule and ipu4-acpi drivers:
- https://github.com/intel/linux-intel-lts/tree/4.19/base/drivers/media/i2c/crlmodule
- https://github.com/intel/linux-intel-lts/blob/4.19/base/drivers/media/platform/intel/ipu4-acpi.c

The crlmodule looks like the common driver for sensors.
"CRL" stands for "common/configurable register list":
- [intel/linux-intel-lts@dc8fd430](https://github.com/intel/linux-intel-lts/commit/dc8fd43018fa26980c43a449d5b273861267fb73)
- [intel/linux-intel-lts@56c075d6](https://github.com/intel/linux-intel-lts/commit/56c075d6b2978a0841aaaffaf978874be5295ba8)

> Common register list based driver is based on a sensor configuration
> file created for a specific sensor module based on a set of
> rules.

Other places providing IPU4 drivers:
- https://github.com/intel/intel-camera-drivers [out-of-tree driver files, outdated]
- https://github.com/clearlinux-pkgs/linux-iot-lts2018 [as patch files]
- https://github.com/clearlinux-pkgs/linux-iot-lts2018-preempt-rt [same as above]

#### Chromebooks

Some Chromebooks use IPU3 with almost the same sensor drivers as upstream. But the sensor driver works anyway because Chromebooks handle power-managements at rather firmware through runtime_pm:
- https://github.com/coreboot/coreboot/blob/master/src/mainboard/google/poppy/variants/baseboard/include/baseboard/acpi/camera_pmic.asl
- https://github.com/torvalds/linux/blob/e9919e11e219eaa5e8041b7b1a196839143e9125/drivers/media/i2c/ov5670.c#L2515-L2518

links to ACPI asl files for devices with IPU3:
- https://github.com/coreboot/coreboot/tree/master/src/mainboard/google/poppy/variants/baseboard/include/baseboard/acpi
- https://github.com/coreboot/coreboot/tree/master/src/mainboard/google/poppy/variants/atlas/include/variant/acpi
- https://github.com/coreboot/coreboot/tree/master/src/mainboard/google/poppy/variants/nautilus/include/variant/acpi
- https://github.com/coreboot/coreboot/tree/master/src/mainboard/google/poppy/variants/nocturne/include/variant/acpi

links to ACPI asl files for devices with IPU4:
- https://github.com/coreboot/coreboot/tree/master/src/mainboard/google/dedede/variants/baseboard/include/baseboard/acpi
- https://github.com/coreboot/coreboot/tree/master/src/mainboard/intel/jasperlake_rvp/variants/baseboard/include/baseboard/acpi
