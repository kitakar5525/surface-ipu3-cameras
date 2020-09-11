Patches for
- allow tps68470 drivers to be built as modules
- add tps68470_i2c_ids to tps68470-mfd for sysfs new_device
- add tps68470-regulator driver
- add tps68470-clk driver

These patches were intended to be used for clk/regulator handling. However,
it turned out later that these handling isn't needed at least for Surface/MIIX-510/Switch-Alpha-12.
So, no need to apply.