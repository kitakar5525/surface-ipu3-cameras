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

Driver is taken from atomisp:
- https://git.linuxtv.org/mchehab/experimental.git/tree/drivers/staging/media/atomisp/i2c/ov5693?h=atomisp_v5&id=2017e8afcee06fd82e7719f523a07f00eedc283b

(Taken from non-upstream tree because atomisp drivers were not upstream at that time yet.)
