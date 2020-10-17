#### result on Acer Switch Alpha 12

Result with tree https://github.com/kitakar5525/surface-ipu3-cameras/tree/c8e38f5cf89c04cc2e0eb11a686f51e6842635f9

```
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
/sys/class/dmi/id/bios_date:04/16/2018
/sys/class/dmi/id/bios_release:1.6
/sys/class/dmi/id/bios_vendor:Insyde Corp.
/sys/class/dmi/id/bios_version:V1.06
/sys/class/dmi/id/board_asset_tag:Type2 - Board Asset Tag
/sys/class/dmi/id/board_name:Hawaii
/sys/class/dmi/id/board_vendor:Acer
/sys/class/dmi/id/board_version:V1.06
/sys/class/dmi/id/chassis_type:30
/sys/class/dmi/id/chassis_vendor:Acer
/sys/class/dmi/id/chassis_version:Chassis Version
/sys/class/dmi/id/ec_firmware_release:1.6
/sys/class/dmi/id/modalias:dmi:bvnInsydeCorp.:bvrV1.06:bd04/16/2018:br1.6:efr1.6:svnAcer:pnSwitchSA5-271:pvrV1.06:rvnAcer:rnHawaii:rvrV1.06:cvnAcer:ct30:cvrChassisVersion:
/sys/class/dmi/id/product_family:SKL
/sys/class/dmi/id/product_name:Switch SA5-271
/sys/class/dmi/id/product_sku:Switch SA5-271_111E_1.06
/sys/class/dmi/id/product_version:V1.06
/sys/class/dmi/id/sys_vendor:Acer
/sys/class/dmi/id/uevent:MODALIAS=dmi:bvnInsydeCorp.:bvrV1.06:bd04/16/2018:br1.6:efr1.6:svnAcer:pnSwitchSA5-271:pvrV1.06:rvnAcer:rnHawaii:rvrV1.06:cvnAcer:ct30:cvrChassisVersion:
[  109.193169] acpi INT3479:00: -------------------- INT3479:00 --------------------
[  109.193533] Sensor name: ov5670
[  109.193535] ACPI path: \_SB_.PCI0.LNK0
[  109.193536] ACPI device name: INT3479:00
[  109.193540] i2c device name: i2c-INT3479:00
[  109.193690] _DEP ACPI path (1 of 2): \_SB_.PCI0.DSC0
[  109.193692] _DEP ACPI path (2 of 2): \_SB_.PCI0.I2C2
[  109.193693] acpi INT3479:00: ========== dump_crs() ==========
[  109.194186] full raw output of _CRS:
[  109.194188] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
[  109.194189] 00000010: 36 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 32  6.\_SB.PCI0.I2C2
[  109.194190] 00000020: 00 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06  ................
[  109.194191] 00000030: 00 0c 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43  ...\_SB.PCI0.I2C
[  109.194192] 00000040: 32 00 79 00                                      2.y.
[  109.194193] acpi INT3479:00: ========== dump_ssdb() ==========
[  109.194194] full raw output of SSDB:
[  109.194195] 00000000: 00 30 69 56 39 8a f7 11 a9 4e 9c 7d 20 ee 0a b5  .0iV9....N.} ...
[  109.194196] 00000010: ca 40 a3 00 00 00 00 00 00 00 00 00 00 02 00 00  .@..............
[  109.194197] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[  109.194197] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[  109.194198] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 04  ................
[  109.194199] 00000050: 09 00 02 00 b4 01 00 f8 24 01 00 00 00 00 00 00  ........$.......
[  109.194200] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[  109.194201] ======= Important data =======
[  109.194201] link_used:     0
[  109.194202] lanes_used:    2
[  109.194202] vcm_type:      4
[  109.194203] flash_support: 2
[  109.194204] degree:        180
[  109.194204] mclk_speed:    19200000
[  109.194205] mclk_port:     0
[  109.194205] ========== All data ==========
[  109.194206] version:                      0
[  109.194207] sensor_card_sku:              48
[  109.194207] csi2_data_stream_interface:
[  109.194208] 00000000: 69 56 39 8a f7 11 a9 4e 9c 7d 20 ee 0a b5 ca 40  iV9....N.} ....@
[  109.194209] bdf_value:                    163
[  109.194210] dphy_link_en_fuses:           0
[  109.194210] lanes_clock_division:         0
[  109.194211] link_used:                    0
[  109.194211] lanes_used:                   2
[  109.194212] csi_rx_dly_cnt_termen_clane:  0
[  109.194213] csi_rx_dly_cnt_settle_clane:  0
[  109.194213] csi_rx_dly_cnt_termen_dlane0: 0
[  109.194214] csi_rx_dly_cnt_settle_dlane0: 0
[  109.194214] csi_rx_dly_cnt_termen_dlane1: 0
[  109.194215] csi_rx_dly_cnt_settle_dlane1: 0
[  109.194216] csi_rx_dly_cnt_termen_dlane2: 0
[  109.194216] csi_rx_dly_cnt_settle_dlane2: 0
[  109.194217] csi_rx_dly_cnt_termen_dlane3: 0
[  109.194217] csi_rx_dly_cnt_settle_dlane3: 0
[  109.194218] max_lane_speed:               0
[  109.194219] sensor_cal_file_idx:          0
[  109.194219] sensor_cal_file_idx_mbz:
[  109.194220] 00000000: 00 00 00                                         ...
[  109.194221] rom_type:                     1
[  109.194221] vcm_type:                     4
[  109.194222] platform:                     9
[  109.194223] platform_sub:                 0
[  109.194223] flash_support:                2
[  109.194224] privacy_led:                  0
[  109.194224] degree:                       180
[  109.194225] mipi_define:                  1
[  109.194226] mclk_speed:                   19200000
[  109.194226] control_logic_id:             0
[  109.194227] mipi_data_format:             0
[  109.194227] silicon_version:              0
[  109.194228] customer_id:                  0
[  109.194229] mclk_port:                    0
[  109.194229] reserved:
[  109.194230] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............
[  109.194260] __dump_subsys_id_dsm(): Subsystem ID: CJAF510
[  109.194320] __dump_i2c_dev_dsm(): i2c device amount: 2
[  109.194494] __dump_i2c_dev_dsm(): i2c device _DSM data (1 of 2): 0x02003600
[  109.194658] __dump_i2c_dev_dsm(): i2c device _DSM data (2 of 2): 0x02000c01
[  109.194689] __dump_discrete_pmic_dsm(): GPIO pin amount: 0
[  109.196072] __dump_dsmb_dsm(): full raw output of DSMB:
[  109.196074] 00000000: 02 00 00 00 00 36 00 02 01 0c 00 02 02 50 00 02  .....6.......P..
[  109.196075] 00000010: 03 51 00 02 04 52 00 02 05 53 00 02 06 54 00 02  .Q...R...S...T..
[  109.196076] 00000020: 07 55 00 02 08 56 00 02 09 57 00 02 0a 58 00 02  .U...V...W...X..
[  109.196076] 00000030: 00 59 00 02                                      .Y..

[  109.196276] acpi INT3472:00: -------------------- INT3472:00 --------------------
[  109.196354] ACPI path: \_SB_.PCI0.DSC0
[  109.196354] ACPI device name: INT3472:00
[  109.196358] print_i2c_dev_name(): i2c device not found. Possible reasons:
[  109.196359] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
[  109.196359] print_i2c_dev_name(): 2) It's a sensor device and the real sensor driver is loaded with the bridge driver.
[  109.196360] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
[  109.196362] print_dep_acpi_paths(): No _DEP entry found
[  109.196366] acpi INT3472:00: ========== dump_crs() ==========
[  109.196752] full raw output of _CRS:
[  109.196754] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
[  109.196755] 00000010: 00 19 00 28 00 00 00 7e 00 5c 5f 53 42 2e 50 43  ...(...~.\_SB.PC
[  109.196756] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
[  109.196757] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 7f  ...........(....
[  109.196758] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
[  109.196759] 00000050: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
[  109.196760] 00000060: 00 19 00 28 00 00 00 8f 00 5c 5f 53 42 2e 50 43  ...(.....\_SB.PC
[  109.196761] 00000070: 49 30 2e 47 50 49 30 00 79 00                    I0.GPI0.y.
[  109.196765] acpi INT3472:00: ========== dump_cldb() ==========
[  109.196765] full raw output of CLDB:
[  109.196766] 00000000: 00 01 00 30 00 00 00 00 00 00 00 00 00 00 00 00  ...0............
[  109.196767] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[  109.196768] version:            0
[  109.196768] control_logic_type: 1
[  109.196769] control_logic_id:   0
[  109.196770] sensor_card_sku:    48
[  109.196770] reserved:
[  109.196771] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[  109.196772] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[  109.196773] print_pmic_type(): PMIC type is "1: DISCRETE(CRD-D)"
[  109.196795] __dump_subsys_id_dsm(): _DSM failed for getting subsystem ID func. Maybe it doesn't exist.
[  109.196815] __dump_i2c_dev_dsm(): i2c device amount: 0
[  109.196867] __dump_discrete_pmic_dsm(): GPIO pin amount: 3
[  109.197043] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (1 of 3): 0x01007e00
[  109.197224] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (2 of 3): 0x01007f0b
[  109.197402] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (3 of 3): 0x01008f0c
[  109.197420] __dump_dsmb_dsm(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.
```
