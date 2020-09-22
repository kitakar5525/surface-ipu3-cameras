#### build

```bash
make
```

#### load

```bash
# If the atomisp version of ov5693 is loaded before our (ipu3 version)
# ov5693, it prevents our ov5693 from loading (because the ACPI ID is the same.)
# So, unload it first if exists:
sudo modprobe -r atomisp_ov5693

sudo insmod ov5693.ko
```

#### References

Driver is taken from jhand2:
- https://github.com/jhand2/surface-camera/blob/1ef74b790906f293d51b6673e06358593534f7bf/patches/0004-Add-ov5693-camera-driver.patch
