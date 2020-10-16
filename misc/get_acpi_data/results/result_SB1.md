#### result on Microsoft Surface Book 1

Result with tree https://github.com/kitakar5525/surface-ipu3-cameras/tree/fe3ff654fb0e20563521300a26901f831be4ead5

```
kern  :info  : [ 2433.469853] acpi INT33BE:00: -------------------- INT33BE:00 --------------------
kern  :info  : [ 2433.469890] Sensor name: ov5693
kern  :info  : [ 2433.469892] ACPI path: \_SB_.PCI0.I2C2.CAMF
kern  :info  : [ 2433.469892] ACPI device name: INT33BE:00
kern  :info  : [ 2433.469897] i2c device name: i2c-INT33BE:00
kern  :info  : [ 2433.469904] _DEP ACPI path (1 of 1): \_SB_.PCI0.I2C2.SKC1
kern  :info  : [ 2433.469905] acpi INT33BE:00: ========== dump_crs() ==========
kern  :info  : [ 2433.469917] full raw output of _CRS:
kern  :info  : [ 2433.469919] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
kern  :info  : [ 2433.469920] 00000010: 36 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 32  6.\_SB.PCI0.I2C2
kern  :info  : [ 2433.469921] 00000020: 00 79 00                                         .y.
kern  :info  : [ 2433.469922] acpi INT33BE:00: ========== dump_ssdb() ==========
kern  :info  : [ 2433.469922] full raw output of SSDB:
kern  :info  : [ 2433.469923] 00000000: 00 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00  . ..............
kern  :info  : [ 2433.469923] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 01 02 00 00  ................
kern  :info  : [ 2433.469924] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.469925] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.469925] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 00  ................
kern  :info  : [ 2433.469926] 00000050: 09 00 02 01 01 01 00 f8 24 01 01 00 00 00 00 00  ........$.......
kern  :info  : [ 2433.469927] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
kern  :info  : [ 2433.469927] ======= Important data =======
kern  :info  : [ 2433.469928] link_used:     1
kern  :info  : [ 2433.469928] lanes_used:    2
kern  :info  : [ 2433.469929] vcm_type:      0
kern  :info  : [ 2433.469929] flash_support: 2
kern  :info  : [ 2433.469930] degree:        1
kern  :info  : [ 2433.469930] mclk_speed:    19200000
kern  :info  : [ 2433.469930] mclk_port:     0
kern  :info  : [ 2433.469931] ========== All data ==========
kern  :info  : [ 2433.469931] version:                      0
kern  :info  : [ 2433.469932] sensor_card_sku:              32
kern  :info  : [ 2433.469932] csi2_data_stream_interface:
kern  :info  : [ 2433.469933] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.469933] bdf_value:                    0
kern  :info  : [ 2433.469934] dphy_link_en_fuses:           0
kern  :info  : [ 2433.469934] lanes_clock_division:         0
kern  :info  : [ 2433.469935] link_used:                    1
kern  :info  : [ 2433.469935] lanes_used:                   2
kern  :info  : [ 2433.469935] csi_rx_dly_cnt_termen_clane:  0
kern  :info  : [ 2433.469936] csi_rx_dly_cnt_settle_clane:  0
kern  :info  : [ 2433.469936] csi_rx_dly_cnt_termen_dlane0: 0
kern  :info  : [ 2433.469937] csi_rx_dly_cnt_settle_dlane0: 0
kern  :info  : [ 2433.469937] csi_rx_dly_cnt_termen_dlane1: 0
kern  :info  : [ 2433.469937] csi_rx_dly_cnt_settle_dlane1: 0
kern  :info  : [ 2433.469938] csi_rx_dly_cnt_termen_dlane2: 0
kern  :info  : [ 2433.469938] csi_rx_dly_cnt_settle_dlane2: 0
kern  :info  : [ 2433.469939] csi_rx_dly_cnt_termen_dlane3: 0
kern  :info  : [ 2433.469939] csi_rx_dly_cnt_settle_dlane3: 0
kern  :info  : [ 2433.469940] max_lane_speed:               0
kern  :info  : [ 2433.469940] sensor_cal_file_idx:          0
kern  :info  : [ 2433.469940] sensor_cal_file_idx_mbz:
kern  :info  : [ 2433.469941] 00000000: 00 00 00                                         ...
kern  :info  : [ 2433.469941] rom_type:                     1
kern  :info  : [ 2433.469942] vcm_type:                     0
kern  :info  : [ 2433.469942] platform:                     9
kern  :info  : [ 2433.469943] platform_sub:                 0
kern  :info  : [ 2433.469943] flash_support:                2
kern  :info  : [ 2433.469944] privacy_led:                  1
kern  :info  : [ 2433.469944] degree:                       1
kern  :info  : [ 2433.469944] mipi_define:                  1
kern  :info  : [ 2433.469945] mclk_speed:                   19200000
kern  :info  : [ 2433.469945] control_logic_id:             1
kern  :info  : [ 2433.469946] mipi_data_format:             0
kern  :info  : [ 2433.469946] silicon_version:              0
kern  :info  : [ 2433.469947] customer_id:                  0
kern  :info  : [ 2433.469947] mclk_port:                    0
kern  :info  : [ 2433.469947] reserved:
kern  :info  : [ 2433.469948] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............

kern  :info  : [ 2433.469969] acpi INT3472:00: -------------------- INT3472:00 --------------------
kern  :info  : [ 2433.469980] ACPI path: \_SB_.PCI0.I2C2.SKC1
kern  :info  : [ 2433.469981] ACPI device name: INT3472:00
kern  :info  : [ 2433.469984] print_i2c_dev_name(): i2c device not found. Possible reasons:
kern  :info  : [ 2433.469984] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
kern  :info  : [ 2433.469984] print_i2c_dev_name(): 2) It's a sensor driver and the real sensor driver is loaded with the bridge driver.
kern  :info  : [ 2433.469985] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
kern  :info  : [ 2433.469986] print_dep_acpi_paths(): No _DEP entry found
kern  :info  : [ 2433.469987] acpi INT3472:00: ========== dump_crs() ==========
kern  :info  : [ 2433.469997] full raw output of _CRS:
kern  :info  : [ 2433.469998] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [ 2433.469999] 00000010: 00 19 00 28 00 00 00 54 00 5c 5f 53 42 2e 50 43  ...(...T.\_SB.PC
kern  :info  : [ 2433.469999] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
kern  :info  : [ 2433.470000] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 4d  ...........(...M
kern  :info  : [ 2433.470001] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
kern  :info  : [ 2433.470001] 00000050: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [ 2433.470002] 00000060: 00 19 00 28 00 00 00 16 00 5c 5f 53 42 2e 50 43  ...(.....\_SB.PC
kern  :info  : [ 2433.470003] 00000070: 49 30 2e 47 50 49 30 00 79 00                    I0.GPI0.y.
kern  :info  : [ 2433.470004] acpi INT3472:00: ========== dump_cldb() ==========
kern  :info  : [ 2433.470004] full raw output of CLDB:
kern  :info  : [ 2433.470005] 00000000: 00 01 01 20 00 00 00 00 00 00 00 00 00 00 00 00  ... ............
kern  :info  : [ 2433.470005] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470006] version:            0
kern  :info  : [ 2433.470006] control_logic_type: 1
kern  :info  : [ 2433.470007] control_logic_id:   1
kern  :info  : [ 2433.470007] sensor_card_sku:    32
kern  :info  : [ 2433.470008] reserved:
kern  :info  : [ 2433.470008] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470009] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............

kern  :info  : [ 2433.470076] acpi INT347A:00: -------------------- INT347A:00 --------------------
kern  :info  : [ 2433.470089] Sensor name: ov8865
kern  :info  : [ 2433.470090] ACPI path: \_SB_.PCI0.I2C3.CAMR
kern  :info  : [ 2433.470090] ACPI device name: INT347A:00
kern  :info  : [ 2433.470093] i2c device name: i2c-INT347A:00
kern  :info  : [ 2433.470097] _DEP ACPI path (1 of 1): \_SB_.PCI0.I2C3.SKC0
kern  :info  : [ 2433.470098] acpi INT347A:00: ========== dump_crs() ==========
kern  :info  : [ 2433.470107] full raw output of _CRS:
kern  :info  : [ 2433.470108] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
kern  :info  : [ 2433.470109] 00000010: 10 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 33  ..\_SB.PCI0.I2C3
kern  :info  : [ 2433.470110] 00000020: 00 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06  ................
kern  :info  : [ 2433.470110] 00000030: 00 0c 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43  ...\_SB.PCI0.I2C
kern  :info  : [ 2433.470111] 00000040: 33 00 79 00                                      3.y.
kern  :info  : [ 2433.470112] acpi INT347A:00: ========== dump_ssdb() ==========
kern  :info  : [ 2433.470112] full raw output of SSDB:
kern  :info  : [ 2433.470113] 00000000: 00 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00  . ..............
kern  :info  : [ 2433.470114] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 04 00 00  ................
kern  :info  : [ 2433.470114] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470115] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470115] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 04  ................
kern  :info  : [ 2433.470116] 00000050: 09 00 02 01 01 01 00 f8 24 01 00 00 00 00 00 00  ........$.......
kern  :info  : [ 2433.470117] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
kern  :info  : [ 2433.470117] ======= Important data =======
kern  :info  : [ 2433.470117] link_used:     0
kern  :info  : [ 2433.470118] lanes_used:    4
kern  :info  : [ 2433.470118] vcm_type:      4
kern  :info  : [ 2433.470119] flash_support: 2
kern  :info  : [ 2433.470119] degree:        1
kern  :info  : [ 2433.470119] mclk_speed:    19200000
kern  :info  : [ 2433.470120] mclk_port:     0
kern  :info  : [ 2433.470120] ========== All data ==========
kern  :info  : [ 2433.470121] version:                      0
kern  :info  : [ 2433.470121] sensor_card_sku:              32
kern  :info  : [ 2433.470121] csi2_data_stream_interface:
kern  :info  : [ 2433.470122] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470122] bdf_value:                    0
kern  :info  : [ 2433.470123] dphy_link_en_fuses:           0
kern  :info  : [ 2433.470123] lanes_clock_division:         0
kern  :info  : [ 2433.470124] link_used:                    0
kern  :info  : [ 2433.470124] lanes_used:                   4
kern  :info  : [ 2433.470125] csi_rx_dly_cnt_termen_clane:  0
kern  :info  : [ 2433.470125] csi_rx_dly_cnt_settle_clane:  0
kern  :info  : [ 2433.470125] csi_rx_dly_cnt_termen_dlane0: 0
kern  :info  : [ 2433.470126] csi_rx_dly_cnt_settle_dlane0: 0
kern  :info  : [ 2433.470126] csi_rx_dly_cnt_termen_dlane1: 0
kern  :info  : [ 2433.470127] csi_rx_dly_cnt_settle_dlane1: 0
kern  :info  : [ 2433.470127] csi_rx_dly_cnt_termen_dlane2: 0
kern  :info  : [ 2433.470127] csi_rx_dly_cnt_settle_dlane2: 0
kern  :info  : [ 2433.470128] csi_rx_dly_cnt_termen_dlane3: 0
kern  :info  : [ 2433.470128] csi_rx_dly_cnt_settle_dlane3: 0
kern  :info  : [ 2433.470129] max_lane_speed:               0
kern  :info  : [ 2433.470129] sensor_cal_file_idx:          0
kern  :info  : [ 2433.470129] sensor_cal_file_idx_mbz:
kern  :info  : [ 2433.470130] 00000000: 00 00 00                                         ...
kern  :info  : [ 2433.470131] rom_type:                     1
kern  :info  : [ 2433.470131] vcm_type:                     4
kern  :info  : [ 2433.470131] platform:                     9
kern  :info  : [ 2433.470132] platform_sub:                 0
kern  :info  : [ 2433.470132] flash_support:                2
kern  :info  : [ 2433.470133] privacy_led:                  1
kern  :info  : [ 2433.470133] degree:                       1
kern  :info  : [ 2433.470134] mipi_define:                  1
kern  :info  : [ 2433.470134] mclk_speed:                   19200000
kern  :info  : [ 2433.470134] control_logic_id:             0
kern  :info  : [ 2433.470135] mipi_data_format:             0
kern  :info  : [ 2433.470135] silicon_version:              0
kern  :info  : [ 2433.470136] customer_id:                  0
kern  :info  : [ 2433.470136] mclk_port:                    0
kern  :info  : [ 2433.470136] reserved:
kern  :info  : [ 2433.470137] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............

kern  :info  : [ 2433.470154] acpi INT3472:01: -------------------- INT3472:01 --------------------
kern  :info  : [ 2433.470164] ACPI path: \_SB_.PCI0.I2C3.SKC0
kern  :info  : [ 2433.470165] ACPI device name: INT3472:01
kern  :info  : [ 2433.470166] print_i2c_dev_name(): i2c device not found. Possible reasons:
kern  :info  : [ 2433.470167] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
kern  :info  : [ 2433.470167] print_i2c_dev_name(): 2) It's a sensor driver and the real sensor driver is loaded with the bridge driver.
kern  :info  : [ 2433.470168] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
kern  :info  : [ 2433.470169] print_dep_acpi_paths(): No _DEP entry found
kern  :info  : [ 2433.470170] acpi INT3472:01: ========== dump_crs() ==========
kern  :info  : [ 2433.470179] full raw output of _CRS:
kern  :info  : [ 2433.470180] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [ 2433.470181] 00000010: 00 19 00 28 00 00 00 53 00 5c 5f 53 42 2e 50 43  ...(...S.\_SB.PC
kern  :info  : [ 2433.470181] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
kern  :info  : [ 2433.470182] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 4e  ...........(...N
kern  :info  : [ 2433.470182] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
kern  :info  : [ 2433.470183] 00000050: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [ 2433.470184] 00000060: 00 19 00 28 00 00 00 2b 00 5c 5f 53 42 2e 50 43  ...(...+.\_SB.PC
kern  :info  : [ 2433.470184] 00000070: 49 30 2e 47 50 49 30 00 79 00                    I0.GPI0.y.
kern  :info  : [ 2433.470185] acpi INT3472:01: ========== dump_cldb() ==========
kern  :info  : [ 2433.470186] full raw output of CLDB:
kern  :info  : [ 2433.470186] 00000000: 00 01 00 20 00 00 00 00 00 00 00 00 00 00 00 00  ... ............
kern  :info  : [ 2433.470187] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470187] version:            0
kern  :info  : [ 2433.470188] control_logic_type: 1
kern  :info  : [ 2433.470188] control_logic_id:   0
kern  :info  : [ 2433.470189] sensor_card_sku:    32
kern  :info  : [ 2433.470189] reserved:
kern  :info  : [ 2433.470190] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470190] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............

kern  :info  : [ 2433.470212] acpi INT347E:00: -------------------- INT347E:00 --------------------
kern  :info  : [ 2433.470222] Sensor name: ov7251
kern  :info  : [ 2433.470223] ACPI path: \_SB_.PCI0.I2C3.CAM3
kern  :info  : [ 2433.470223] ACPI device name: INT347E:00
kern  :info  : [ 2433.470225] i2c device name: i2c-INT347E:00
kern  :info  : [ 2433.470229] _DEP ACPI path (1 of 1): \_SB_.PCI0.I2C3.SKC2
kern  :info  : [ 2433.470230] acpi INT347E:00: ========== dump_crs() ==========
kern  :info  : [ 2433.470247] full raw output of _CRS:
kern  :info  : [ 2433.470249] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
kern  :info  : [ 2433.470250] 00000010: 60 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 33  `.\_SB.PCI0.I2C3
kern  :info  : [ 2433.470251] 00000020: 00 79 00                                         .y.
kern  :info  : [ 2433.470252] acpi INT347E:00: ========== dump_ssdb() ==========
kern  :info  : [ 2433.470253] full raw output of SSDB:
kern  :info  : [ 2433.470254] 00000000: 00 20 00 00 00 00 00 00 00 00 00 00 00 00 00 00  . ..............
kern  :info  : [ 2433.470255] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 02 01 00 00  ................
kern  :info  : [ 2433.470255] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470256] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470257] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470258] 00000050: 09 00 02 00 00 01 00 f8 24 01 02 00 00 00 00 00  ........$.......
kern  :info  : [ 2433.470259] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
kern  :info  : [ 2433.470260] ======= Important data =======
kern  :info  : [ 2433.470261] link_used:     2
kern  :info  : [ 2433.470261] lanes_used:    1
kern  :info  : [ 2433.470262] vcm_type:      0
kern  :info  : [ 2433.470263] flash_support: 2
kern  :info  : [ 2433.470263] degree:        0
kern  :info  : [ 2433.470264] mclk_speed:    19200000
kern  :info  : [ 2433.470264] mclk_port:     0
kern  :info  : [ 2433.470265] ========== All data ==========
kern  :info  : [ 2433.470265] version:                      0
kern  :info  : [ 2433.470266] sensor_card_sku:              32
kern  :info  : [ 2433.470266] csi2_data_stream_interface:
kern  :info  : [ 2433.470267] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470267] bdf_value:                    0
kern  :info  : [ 2433.470268] dphy_link_en_fuses:           0
kern  :info  : [ 2433.470269] lanes_clock_division:         0
kern  :info  : [ 2433.470269] link_used:                    2
kern  :info  : [ 2433.470270] lanes_used:                   1
kern  :info  : [ 2433.470270] csi_rx_dly_cnt_termen_clane:  0
kern  :info  : [ 2433.470271] csi_rx_dly_cnt_settle_clane:  0
kern  :info  : [ 2433.470271] csi_rx_dly_cnt_termen_dlane0: 0
kern  :info  : [ 2433.470272] csi_rx_dly_cnt_settle_dlane0: 0
kern  :info  : [ 2433.470273] csi_rx_dly_cnt_termen_dlane1: 0
kern  :info  : [ 2433.470273] csi_rx_dly_cnt_settle_dlane1: 0
kern  :info  : [ 2433.470274] csi_rx_dly_cnt_termen_dlane2: 0
kern  :info  : [ 2433.470274] csi_rx_dly_cnt_settle_dlane2: 0
kern  :info  : [ 2433.470275] csi_rx_dly_cnt_termen_dlane3: 0
kern  :info  : [ 2433.470275] csi_rx_dly_cnt_settle_dlane3: 0
kern  :info  : [ 2433.470276] max_lane_speed:               0
kern  :info  : [ 2433.470276] sensor_cal_file_idx:          0
kern  :info  : [ 2433.470277] sensor_cal_file_idx_mbz:
kern  :info  : [ 2433.470278] 00000000: 00 00 00                                         ...
kern  :info  : [ 2433.470279] rom_type:                     0
kern  :info  : [ 2433.470279] vcm_type:                     0
kern  :info  : [ 2433.470280] platform:                     9
kern  :info  : [ 2433.470280] platform_sub:                 0
kern  :info  : [ 2433.470281] flash_support:                2
kern  :info  : [ 2433.470282] privacy_led:                  0
kern  :info  : [ 2433.470282] degree:                       0
kern  :info  : [ 2433.470283] mipi_define:                  1
kern  :info  : [ 2433.470284] mclk_speed:                   19200000
kern  :info  : [ 2433.470284] control_logic_id:             2
kern  :info  : [ 2433.470285] mipi_data_format:             0
kern  :info  : [ 2433.470285] silicon_version:              0
kern  :info  : [ 2433.470286] customer_id:                  0
kern  :info  : [ 2433.470286] mclk_port:                    0
kern  :info  : [ 2433.470287] reserved:
kern  :info  : [ 2433.470288] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............

kern  :info  : [ 2433.470307] acpi INT3472:02: -------------------- INT3472:02 --------------------
kern  :info  : [ 2433.470320] ACPI path: \_SB_.PCI0.I2C3.SKC2
kern  :info  : [ 2433.470320] ACPI device name: INT3472:02
kern  :info  : [ 2433.470322] print_i2c_dev_name(): i2c device not found. Possible reasons:
kern  :info  : [ 2433.470323] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
kern  :info  : [ 2433.470323] print_i2c_dev_name(): 2) It's a sensor driver and the real sensor driver is loaded with the bridge driver.
kern  :info  : [ 2433.470324] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
kern  :info  : [ 2433.470325] print_dep_acpi_paths(): No _DEP entry found
kern  :info  : [ 2433.470326] acpi INT3472:02: ========== dump_crs() ==========
kern  :info  : [ 2433.470335] full raw output of _CRS:
kern  :info  : [ 2433.470336] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [ 2433.470336] 00000010: 00 19 00 28 00 00 00 4f 00 5c 5f 53 42 2e 50 43  ...(...O.\_SB.PC
kern  :info  : [ 2433.470337] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
kern  :info  : [ 2433.470338] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 50  ...........(...P
kern  :info  : [ 2433.470338] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
kern  :info  : [ 2433.470339] 00000050: 79 00                                            y.
kern  :info  : [ 2433.470340] acpi INT3472:02: ========== dump_cldb() ==========
kern  :info  : [ 2433.470340] full raw output of CLDB:
kern  :info  : [ 2433.470341] 00000000: 00 01 02 20 00 00 00 00 00 00 00 00 00 00 00 00  ... ............
kern  :info  : [ 2433.470342] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470342] version:            0
kern  :info  : [ 2433.470343] control_logic_type: 1
kern  :info  : [ 2433.470343] control_logic_id:   2
kern  :info  : [ 2433.470343] sensor_card_sku:    32
kern  :info  : [ 2433.470344] reserved:
kern  :info  : [ 2433.470344] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [ 2433.470345] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............
```