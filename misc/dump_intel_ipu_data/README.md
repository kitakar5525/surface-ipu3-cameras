#### build
Prerequisites: `headers` package for your kernel
```bash
make
```

#### loading module and getting data
Copy-paste and run the following script in the same directory as the
module you built exists.
The log file will be saved to your home directory.

```bash
MOD_NAME="dump_intel_ipu_data"
mod_version=$(modinfo ${MOD_NAME}.ko | awk '/^version:.*$/ {print $2}')
product_name=$(cat /sys/class/dmi/id/product_name | sed s/'\s'/'_'/g)
product_sku=$(cat /sys/class/dmi/id/product_sku | sed s/'\s'/'_'/g)
log_location=~/result_${product_name}_${product_sku}@v${mod_version}.md

# Print your system info:
# - do not use sudo so that it doesn't contain personal data. So,
#   "Permission denied" is expected for such data.
#
print_system_info ()
{
    grep . /sys/class/dmi/id/*

    # Get CPU model (script from neofetch)
    awk -F '\\s*: | @' \
    '/model name|Hardware|Processor|^cpu model|chip type|^cpu type/ {
    cpu=$2; if ($1 == "Hardware") exit } END { print cpu }' "/proc/cpuinfo"

    cat /etc/os-release | grep "NAME"
    echo "uname -r: $(uname -r)"
}

dump_data_to_dmesg ()
{
    # clear the kernel ring buffer so that dmesg doesn't contain
    # unnecessary log
    sudo dmesg -C

    # load/unload module
    sudo insmod ${MOD_NAME}.ko
    sleep 1
    sudo rmmod ${MOD_NAME}
}

dump_data_to_dmesg

# print log to your display
print_system_info
sudo dmesg -x

# save log to file
echo '```'        &> ${log_location} # add markdown code block
print_system_info &>> ${log_location}
echo ""           &>> ${log_location}
sudo dmesg -x     &>> ${log_location}
echo '```'        &>> ${log_location} # add markdown code block
```

When you post the output somewhere. please remove unrelated output like
`audit` output.
You can also temporarily disable the `audit` output:
```bash
# disable audit:
sudo auditctl -e 0
# to enable again:
sudo auditctl -e 1
```

#### kernel lockdown is enabled and I can't do `insmod`
You need to sign the module manually. Alternatively, you can also just
temporarily disable the lockdown or Secure Boot.

#### debug level output
If you need, pass `dyndbg` option to `insmod`/`modprobe` to show debug
level output. For example:
```bash
sudo insmod ${MOD_NAME}.ko dyndbg
```

#### Contribution
Contributions of the log files are welcome! To do so, add vendor name
and product name at the top of the result file.
For example: `Result on Microsoft Surface Book 1`.
Then, copy your result file into `results` directory and make a pull request.
You can also just open a new issue on this repo and attach the result
file there.

I also opened an issue on the linux-surface repo to open a discussion
more widely: https://github.com/linux-surface/linux-surface/issues/326.
You can also attach your result there.

---

#### Note: to those who built your kernel with the old bridge driver (RFC v2 version or below)
To print i2c device name, you still need to unload the real sensor drivers
before loading this module (not mandatory for the other features though).
Details: https://github.com/kitakar5525/surface-ipu3-cameras/commit/582c4ba3bba5812024f5efd33769c6eafd50f371#diff-7e53e40ecaeff527b241d9f9eaec4af11778953028b4bf510d61c9d34c3b4e70R394-R401

Unload the real sensor drivers like the following:
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
