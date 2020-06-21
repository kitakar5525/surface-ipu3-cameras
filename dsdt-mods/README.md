#### DSDT mods

Take a look at ArchWiki [1] for compiling and overriding DSDT.

Here is the one-liner:
```bash
# compile
iasl -tc dsdt.dsl

# make a CPIO archive then copy it into /boot
mkdir -p kernel/firmware/acpi
cp dsdt.aml kernel/firmware/acpi
find kernel | cpio -H newc --create > acpi_override
sudo cp acpi_override /boot

# then, configure your bootloader to load the CPIO archive
```

[1] https://wiki.archlinux.org/index.php/DSDT


#### DSDT mods for IPU3

Some chromebooks use IPU3 with _DSD extensions [2]. It may be helpful understanding what needs to be done.

References:
- https://www.mail-archive.com/linux-media@vger.kernel.org/msg133794.html
  ("Re: Software-only image processing for Intel "complex" cameras")
- https://www.mail-archive.com/linux-media@vger.kernel.org/msg122947.html
  ("webcams not recognized on a Dell Latitude 5285 detachable laptop")
- https://github.com/torvalds/linux/blob/6f0d349d922ba44e4348a17a78ea51b7135965b1/Documentation/devicetree/bindings/media/i2c/ov7251.txt
  [usage example of ov7251 in DT]
- https://github.com/torvalds/linux/blob/435faf5c218a47fd6258187f62d9bb1009717896/arch/arm64/boot/dts/qcom/sdm845-db845c.dts#L1034-L1075
  [real usage of ov7251 in DT]
- [2] https://github.com/coreboot/coreboot/tree/6b5bc77c9b22c398262ff3f4dae3e14904c57366/src/mainboard/google/poppy/variants/nocturne/include/variant/acpi
