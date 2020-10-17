#### result on Microsoft Surface Book 1

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
/sys/class/dmi/id/bios_date:03.24.2020
/sys/class/dmi/id/bios_vendor:Microsoft Corporation
/sys/class/dmi/id/bios_version:92.3192.768
/sys/class/dmi/id/board_name:Surface Book
/sys/class/dmi/id/board_vendor:Microsoft Corporation
/sys/class/dmi/id/chassis_type:9
/sys/class/dmi/id/chassis_vendor:Microsoft Corporation
/sys/class/dmi/id/modalias:dmi:bvnMicrosoftCorporation:bvr92.3192.768:bd03.24.2020:svnMicrosoftCorporation:pnSurfaceBook:pvr124000000000000000000000D0B09F1C03P38:rvnMicrosoftCorporation:rnSurfaceBook:rvr:cvnMicrosoftCorporation:ct9:cvr:
/sys/class/dmi/id/product_family:Surface
/sys/class/dmi/id/product_name:Surface Book
/sys/class/dmi/id/product_sku:Surface_Book
/sys/class/dmi/id/product_version:124000000000000000000000D:0B:09F:1C:03P:38
/sys/class/dmi/id/sys_vendor:Microsoft Corporation
/sys/class/dmi/id/uevent:MODALIAS=dmi:bvnMicrosoftCorporation:bvr92.3192.768:bd03.24.2020:svnMicrosoftCorporation:pnSurfaceBook:pvr124000000000000000000000D0B09F1C03P38:rvnMicrosoftCorporation:rnSurfaceBook:rvr:cvnMicrosoftCorporation:ct9:cvr:
[80280.938931] acpi INT33BE:00: -------------------- INT33BE:00 --------------------
[80280.939013] Sensor name: ov5693
[80280.939015] ACPI path: \_SB_.PCI0.I2C2.CAMF
[80280.939015] ACPI device name: INT33BE:00
[80280.939025] i2c device name: i2c-INT33BE:00
[80280.939036] _DEP ACPI path (1 of 1): \_SB_.PCI0.I2C2.SKC1
[80280.939039] acpi INT33BE:00: ========== dump_crs() ==========
[80280.939061] full raw output of _CRS:
[80280.939064] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
[80280.939065] 00000010: 36 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 32  6.\_SB.PCI0.I2C2
[80280.939065] 00000020: 00 79 00                                         .y.
[80280.939067] acpi INT33BE:00: ========== dump_ssdb() ==========
[80280.939067] full raw output of SSDB:
[80280.939068] 00000000: 00 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00  . ..............
[80280.939069] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 01 02 00 00  ................
[80280.939071] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939072] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939073] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00  ................
[80280.939074] 00000050: 09 00 02 01 01 01 00 f8 24 01 01 00 00 00 00 00  ........$.......
[80280.939075] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[80280.939076] ======= Important data =======
[80280.939077] link_used:     1
[80280.939077] lanes_used:    2
[80280.939078] vcm_type:      0
[80280.939078] flash_support: 2
[80280.939079] degree:        1
[80280.939080] mclk_speed:    19200000
[80280.939080] mclk_port:     0
[80280.939081] ========== All data ==========
[80280.939081] version:                      0
[80280.939082] sensor_card_sku:              32
[80280.939082] csi2_data_stream_interface:
[80280.939084] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939085] bdf_value:                    0
[80280.939085] dphy_link_en_fuses:           0
[80280.939086] lanes_clock_division:         0
[80280.939087] link_used:                    1
[80280.939087] lanes_used:                   2
[80280.939088] csi_rx_dly_cnt_termen_clane:  0
[80280.939088] csi_rx_dly_cnt_settle_clane:  0
[80280.939089] csi_rx_dly_cnt_termen_dlane0: 0
[80280.939089] csi_rx_dly_cnt_settle_dlane0: 0
[80280.939090] csi_rx_dly_cnt_termen_dlane1: 0
[80280.939090] csi_rx_dly_cnt_settle_dlane1: 0
[80280.939091] csi_rx_dly_cnt_termen_dlane2: 0
[80280.939091] csi_rx_dly_cnt_settle_dlane2: 0
[80280.939092] csi_rx_dly_cnt_termen_dlane3: 0
[80280.939093] csi_rx_dly_cnt_settle_dlane3: 0
[80280.939093] max_lane_speed:               0
[80280.939094] sensor_cal_file_idx:          0
[80280.939094] sensor_cal_file_idx_mbz:
[80280.939095] 00000000: 00 00 00                                         ...
[80280.939096] rom_type:                     1
[80280.939096] vcm_type:                     0
[80280.939097] platform:                     9
[80280.939097] platform_sub:                 0
[80280.939098] flash_support:                2
[80280.939099] privacy_led:                  1
[80280.939099] degree:                       1
[80280.939100] mipi_define:                  1
[80280.939100] mclk_speed:                   19200000
[80280.939101] control_logic_id:             1
[80280.939102] mipi_data_format:             0
[80280.939102] silicon_version:              0
[80280.939103] customer_id:                  0
[80280.939104] mclk_port:                    0
[80280.939104] reserved:
[80280.939105] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............
[80280.939153] __dump_subsys_id_dsm(): Subsystem ID: MSHW0070
[80280.939200] __dump_i2c_dev_dsm(): i2c device amount: 1
[80280.939247] __dump_i2c_dev_dsm(): i2c device _DSM data (1 of 1): 0x02003600
[80280.939274] __dump_discrete_pmic_dsm(): GPIO pin amount: 0
[80280.939300] __dump_dsmb_dsm(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.

[80280.939334] acpi INT3472:00: -------------------- INT3472:00 --------------------
[80280.939354] ACPI path: \_SB_.PCI0.I2C2.SKC1
[80280.939355] ACPI device name: INT3472:00
[80280.939361] print_i2c_dev_name(): i2c device not found. Possible reasons:
[80280.939362] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
[80280.939363] print_i2c_dev_name(): 2) It's a sensor device and the real sensor driver is loaded with the bridge driver.
[80280.939364] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
[80280.939365] print_dep_acpi_paths(): No _DEP entry found
[80280.939366] acpi INT3472:00: ========== dump_crs() ==========
[80280.939382] full raw output of _CRS:
[80280.939384] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
[80280.939386] 00000010: 00 19 00 28 00 00 00 54 00 5c 5f 53 42 2e 50 43  ...(...T.\_SB.PC
[80280.939387] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
[80280.939388] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 4d  ...........(...M
[80280.939389] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
[80280.939390] 00000050: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
[80280.939391] 00000060: 00 19 00 28 00 00 00 16 00 5c 5f 53 42 2e 50 43  ...(.....\_SB.PC
[80280.939392] 00000070: 49 30 2e 47 50 49 30 00 79 00                    I0.GPI0.y.
[80280.939393] acpi INT3472:00: ========== dump_cldb() ==========
[80280.939394] full raw output of CLDB:
[80280.939395] 00000000: 00 01 01 20 00 00 00 00 00 00 00 00 00 00 00 00  ... ............
[80280.939395] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939396] version:            0
[80280.939397] control_logic_type: 1
[80280.939397] control_logic_id:   1
[80280.939398] sensor_card_sku:    32
[80280.939398] reserved:
[80280.939399] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939400] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[80280.939401] print_pmic_type(): PMIC type is "1: DISCRETE(CRD-D)"
[80280.939426] __dump_subsys_id_dsm(): _DSM failed for getting subsystem ID func. Maybe it doesn't exist.
[80280.939448] __dump_i2c_dev_dsm(): i2c device amount: 0
[80280.939483] __dump_discrete_pmic_dsm(): GPIO pin amount: 3
[80280.939526] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (1 of 3): 0x0100540c
[80280.939575] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (2 of 3): 0x01004d00
[80280.939627] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (3 of 3): 0x0100160d
[80280.939648] __dump_dsmb_dsm(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.

[80280.939840] acpi INT347A:00: -------------------- INT347A:00 --------------------
[80280.939878] Sensor name: ov8865
[80280.939880] ACPI path: \_SB_.PCI0.I2C3.CAMR
[80280.939880] ACPI device name: INT347A:00
[80280.939886] i2c device name: i2c-INT347A:00
[80280.939896] _DEP ACPI path (1 of 1): \_SB_.PCI0.I2C3.SKC0
[80280.939897] acpi INT347A:00: ========== dump_crs() ==========
[80280.939913] full raw output of _CRS:
[80280.939915] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
[80280.939915] 00000010: 10 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 33  ..\_SB.PCI0.I2C3
[80280.939916] 00000020: 00 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06  ................
[80280.939917] 00000030: 00 0c 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43  ...\_SB.PCI0.I2C
[80280.939918] 00000040: 33 00 79 00                                      3.y.
[80280.939919] acpi INT347A:00: ========== dump_ssdb() ==========
[80280.939920] full raw output of SSDB:
[80280.939921] 00000000: 00 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00  . ..............
[80280.939921] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00  ................
[80280.939922] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939923] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939924] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 04  ................
[80280.939925] 00000050: 09 00 02 01 01 01 00 f8 24 01 00 00 00 00 00 00  ........$.......
[80280.939926] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[80280.939926] ======= Important data =======
[80280.939927] link_used:     0
[80280.939928] lanes_used:    4
[80280.939928] vcm_type:      4
[80280.939929] flash_support: 2
[80280.939929] degree:        1
[80280.939930] mclk_speed:    19200000
[80280.939930] mclk_port:     0
[80280.939931] ========== All data ==========
[80280.939932] version:                      0
[80280.939932] sensor_card_sku:              32
[80280.939933] csi2_data_stream_interface:
[80280.939934] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.939934] bdf_value:                    0
[80280.939935] dphy_link_en_fuses:           0
[80280.939935] lanes_clock_division:         0
[80280.939936] link_used:                    0
[80280.939936] lanes_used:                   4
[80280.939937] csi_rx_dly_cnt_termen_clane:  0
[80280.939937] csi_rx_dly_cnt_settle_clane:  0
[80280.939938] csi_rx_dly_cnt_termen_dlane0: 0
[80280.939938] csi_rx_dly_cnt_settle_dlane0: 0
[80280.939939] csi_rx_dly_cnt_termen_dlane1: 0
[80280.939939] csi_rx_dly_cnt_settle_dlane1: 0
[80280.939940] csi_rx_dly_cnt_termen_dlane2: 0
[80280.939940] csi_rx_dly_cnt_settle_dlane2: 0
[80280.939941] csi_rx_dly_cnt_termen_dlane3: 0
[80280.939942] csi_rx_dly_cnt_settle_dlane3: 0
[80280.939942] max_lane_speed:               0
[80280.939943] sensor_cal_file_idx:          0
[80280.939943] sensor_cal_file_idx_mbz:
[80280.939944] 00000000: 00 00 00                                         ...
[80280.939945] rom_type:                     1
[80280.939945] vcm_type:                     4
[80280.939946] platform:                     9
[80280.939946] platform_sub:                 0
[80280.939947] flash_support:                2
[80280.939947] privacy_led:                  1
[80280.939948] degree:                       1
[80280.939949] mipi_define:                  1
[80280.939949] mclk_speed:                   19200000
[80280.939950] control_logic_id:             0
[80280.939950] mipi_data_format:             0
[80280.939951] silicon_version:              0
[80280.939951] customer_id:                  0
[80280.939952] mclk_port:                    0
[80280.939952] reserved:
[80280.939953] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............
[80280.939986] __dump_subsys_id_dsm(): Subsystem ID: MSHW0071
[80280.940012] __dump_i2c_dev_dsm(): i2c device amount: 2
[80280.940041] __dump_i2c_dev_dsm(): i2c device _DSM data (1 of 2): 0x03001000
[80280.940072] __dump_i2c_dev_dsm(): i2c device _DSM data (2 of 2): 0x03000c01
[80280.940089] __dump_discrete_pmic_dsm(): GPIO pin amount: 0
[80280.940107] __dump_dsmb_dsm(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.

[80280.940143] acpi INT3472:01: -------------------- INT3472:01 --------------------
[80280.940154] ACPI path: \_SB_.PCI0.I2C3.SKC0
[80280.940155] ACPI device name: INT3472:01
[80280.940157] print_i2c_dev_name(): i2c device not found. Possible reasons:
[80280.940158] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
[80280.940158] print_i2c_dev_name(): 2) It's a sensor device and the real sensor driver is loaded with the bridge driver.
[80280.940159] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
[80280.940160] print_dep_acpi_paths(): No _DEP entry found
[80280.940160] acpi INT3472:01: ========== dump_crs() ==========
[80280.940170] full raw output of _CRS:
[80280.940171] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
[80280.940172] 00000010: 00 19 00 28 00 00 00 53 00 5c 5f 53 42 2e 50 43  ...(...S.\_SB.PC
[80280.940172] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
[80280.940173] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 4e  ...........(...N
[80280.940174] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
[80280.940174] 00000050: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
[80280.940175] 00000060: 00 19 00 28 00 00 00 2b 00 5c 5f 53 42 2e 50 43  ...(...+.\_SB.PC
[80280.940175] 00000070: 49 30 2e 47 50 49 30 00 79 00                    I0.GPI0.y.
[80280.940177] acpi INT3472:01: ========== dump_cldb() ==========
[80280.940177] full raw output of CLDB:
[80280.940178] 00000000: 00 01 00 20 00 00 00 00 00 00 00 00 00 00 00 00  ... ............
[80280.940178] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940179] version:            0
[80280.940179] control_logic_type: 1
[80280.940180] control_logic_id:   0
[80280.940180] sensor_card_sku:    32
[80280.940181] reserved:
[80280.940181] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940182] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[80280.940182] print_pmic_type(): PMIC type is "1: DISCRETE(CRD-D)"
[80280.940195] __dump_subsys_id_dsm(): _DSM failed for getting subsystem ID func. Maybe it doesn't exist.
[80280.940207] __dump_i2c_dev_dsm(): i2c device amount: 0
[80280.940230] __dump_discrete_pmic_dsm(): GPIO pin amount: 3
[80280.940255] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (1 of 3): 0x0100530c
[80280.940285] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (2 of 3): 0x01004e00
[80280.940316] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (3 of 3): 0x01002b0d
[80280.940330] __dump_dsmb_dsm(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.

[80280.940394] acpi INT347E:00: -------------------- INT347E:00 --------------------
[80280.940433] Sensor name: ov7251
[80280.940434] ACPI path: \_SB_.PCI0.I2C3.CAM3
[80280.940434] ACPI device name: INT347E:00
[80280.940437] i2c device name: i2c-INT347E:00
[80280.940443] _DEP ACPI path (1 of 1): \_SB_.PCI0.I2C3.SKC2
[80280.940444] acpi INT347E:00: ========== dump_crs() ==========
[80280.940455] full raw output of _CRS:
[80280.940457] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
[80280.940458] 00000010: 60 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 33  `.\_SB.PCI0.I2C3
[80280.940458] 00000020: 00 79 00                                         .y.
[80280.940459] acpi INT347E:00: ========== dump_ssdb() ==========
[80280.940460] full raw output of SSDB:
[80280.940460] 00000000: 00 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00  . ..............
[80280.940461] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 02 01 00 00  ................
[80280.940462] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940462] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940463] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940464] 00000050: 09 00 02 00 00 01 00 f8 24 01 02 00 00 00 00 00  ........$.......
[80280.940464] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[80280.940465] ======= Important data =======
[80280.940465] link_used:     2
[80280.940466] lanes_used:    1
[80280.940466] vcm_type:      0
[80280.940467] flash_support: 2
[80280.940467] degree:        0
[80280.940467] mclk_speed:    19200000
[80280.940468] mclk_port:     0
[80280.940468] ========== All data ==========
[80280.940469] version:                      0
[80280.940469] sensor_card_sku:              32
[80280.940469] csi2_data_stream_interface:
[80280.940470] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940471] bdf_value:                    0
[80280.940471] dphy_link_en_fuses:           0
[80280.940472] lanes_clock_division:         0
[80280.940472] link_used:                    2
[80280.940472] lanes_used:                   1
[80280.940473] csi_rx_dly_cnt_termen_clane:  0
[80280.940473] csi_rx_dly_cnt_settle_clane:  0
[80280.940474] csi_rx_dly_cnt_termen_dlane0: 0
[80280.940474] csi_rx_dly_cnt_settle_dlane0: 0
[80280.940475] csi_rx_dly_cnt_termen_dlane1: 0
[80280.940475] csi_rx_dly_cnt_settle_dlane1: 0
[80280.940475] csi_rx_dly_cnt_termen_dlane2: 0
[80280.940476] csi_rx_dly_cnt_settle_dlane2: 0
[80280.940476] csi_rx_dly_cnt_termen_dlane3: 0
[80280.940477] csi_rx_dly_cnt_settle_dlane3: 0
[80280.940477] max_lane_speed:               0
[80280.940477] sensor_cal_file_idx:          0
[80280.940478] sensor_cal_file_idx_mbz:
[80280.940478] 00000000: 00 00 00                                         ...
[80280.940479] rom_type:                     0
[80280.940479] vcm_type:                     0
[80280.940480] platform:                     9
[80280.940480] platform_sub:                 0
[80280.940480] flash_support:                2
[80280.940481] privacy_led:                  0
[80280.940481] degree:                       0
[80280.940482] mipi_define:                  1
[80280.940482] mclk_speed:                   19200000
[80280.940482] control_logic_id:             2
[80280.940483] mipi_data_format:             0
[80280.940483] silicon_version:              0
[80280.940484] customer_id:                  0
[80280.940484] mclk_port:                    0
[80280.940484] reserved:
[80280.940485] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............
[80280.940513] __dump_subsys_id_dsm(): Subsystem ID: MSHW0072
[80280.940544] __dump_i2c_dev_dsm(): i2c device amount: 1
[80280.940578] __dump_i2c_dev_dsm(): i2c device _DSM data (1 of 1): 0x03006000
[80280.940598] __dump_discrete_pmic_dsm(): GPIO pin amount: 0
[80280.940616] __dump_dsmb_dsm(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.

[80280.940638] acpi INT3472:02: -------------------- INT3472:02 --------------------
[80280.940654] ACPI path: \_SB_.PCI0.I2C3.SKC2
[80280.940654] ACPI device name: INT3472:02
[80280.940657] print_i2c_dev_name(): i2c device not found. Possible reasons:
[80280.940658] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
[80280.940658] print_i2c_dev_name(): 2) It's a sensor device and the real sensor driver is loaded with the bridge driver.
[80280.940659] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
[80280.940660] print_dep_acpi_paths(): No _DEP entry found
[80280.940661] acpi INT3472:02: ========== dump_crs() ==========
[80280.940674] full raw output of _CRS:
[80280.940675] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
[80280.940676] 00000010: 00 19 00 28 00 00 00 4f 00 5c 5f 53 42 2e 50 43  ...(...O.\_SB.PC
[80280.940676] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
[80280.940677] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 50  ...........(...P
[80280.940678] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
[80280.940679] 00000050: 79 00                                            y.
[80280.940680] acpi INT3472:02: ========== dump_cldb() ==========
[80280.940680] full raw output of CLDB:
[80280.940681] 00000000: 00 01 02 20 00 00 00 00 00 00 00 00 00 00 00 00  ... ............
[80280.940682] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940682] version:            0
[80280.940682] control_logic_type: 1
[80280.940683] control_logic_id:   2
[80280.940683] sensor_card_sku:    32
[80280.940684] reserved:
[80280.940685] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
[80280.940685] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............
[80280.940686] print_pmic_type(): PMIC type is "1: DISCRETE(CRD-D)"
[80280.940705] __dump_subsys_id_dsm(): _DSM failed for getting subsystem ID func. Maybe it doesn't exist.
[80280.940725] __dump_i2c_dev_dsm(): i2c device amount: 0
[80280.940750] __dump_discrete_pmic_dsm(): GPIO pin amount: 2
[80280.940780] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (1 of 2): 0x01004f0c
[80280.940812] __dump_discrete_pmic_dsm(): GPIO pin _DSM data (2 of 2): 0x01005000
[80280.940826] __dump_dsmb_dsm(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.
```