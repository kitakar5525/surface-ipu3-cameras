This ov5670 driver is not used by Surface devices but used by e.g.,
Acer Switch Alpha 12.

#### build

```bash
make
```

#### load

```bash
# the upstream version may be already loaded, remove it first
sudo modprobe -r ov5670

sudo insmod ov5670.ko
```

#### References

Driver is taken from upstream:
- https://github.com/torvalds/linux/blob/eba08021e15076afc21b506e71e2f4e523f27f8c/drivers/media/i2c/ov5670.c
