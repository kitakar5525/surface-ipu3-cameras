#### result on Acer Switch Alpha 12

Result with tree https://github.com/kitakar5525/surface-ipu3-cameras/tree/2b46d0671bcf80b9a45147dc09f50dfde21553dc

```
kern  :info  : [  358.633986] acpi INT3479:00: -------------------- INT3479:00 --------------------
kern  :info  : [  358.634300] Sensor name: ov5670
kern  :info  : [  358.634302] ACPI path: \_SB_.PCI0.LNK0
kern  :info  : [  358.634303] ACPI device name: INT3479:00
kern  :info  : [  358.634307] i2c device name: i2c-INT3479:00
kern  :info  : [  358.634425] _DEP ACPI path (1 of 2): \_SB_.PCI0.DSC0
kern  :info  : [  358.634426] _DEP ACPI path (2 of 2): \_SB_.PCI0.I2C2
kern  :info  : [  358.634428] acpi INT3479:00: ========== dump_crs() ==========
kern  :info  : [  358.634905] full raw output of _CRS:
kern  :info  : [  358.634907] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
kern  :info  : [  358.634909] 00000010: 36 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 32  6.\_SB.PCI0.I2C2
kern  :info  : [  358.634910] 00000020: 00 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06  ................
kern  :info  : [  358.634911] 00000030: 00 0c 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43  ...\_SB.PCI0.I2C
kern  :info  : [  358.634912] 00000040: 32 00 79 00                                      2.y.
kern  :info  : [  358.634914] acpi INT3479:00: ========== dump_ssdb() ==========
kern  :info  : [  358.634914] full raw output of SSDB:
kern  :info  : [  358.634915] 00000000: 00 30 69 56 39 8a f7 11 a9 4e 9c 7d 20 ee 0a b5  .0iV9....N.} ...
kern  :info  : [  358.634916] 00000010: ca 40 a3 00 00 00 00 00 00 00 00 00 00 02 00 00  .@..............
kern  :info  : [  358.634917] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  358.634918] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  358.634919] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 04  ................
kern  :info  : [  358.634920] 00000050: 09 00 02 00 b4 01 00 f8 24 01 00 00 00 00 00 00  ........$.......
kern  :info  : [  358.634921] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
kern  :info  : [  358.634921] ======= Important data =======
kern  :info  : [  358.634922] link_used:     0
kern  :info  : [  358.634923] lanes_used:    2
kern  :info  : [  358.634923] vcm_type:      4
kern  :info  : [  358.634924] flash_support: 2
kern  :info  : [  358.634924] degree:        180
kern  :info  : [  358.634925] mclk_speed:    19200000
kern  :info  : [  358.634926] mclk_port:     0
kern  :info  : [  358.634927] ========== All data ==========
kern  :info  : [  358.634927] version:                      0
kern  :info  : [  358.634928] sensor_card_sku:              48
kern  :info  : [  358.634928] csi2_data_stream_interface:
kern  :info  : [  358.634929] 00000000: 69 56 39 8a f7 11 a9 4e 9c 7d 20 ee 0a b5 ca 40  iV9....N.} ....@
kern  :info  : [  358.634930] bdf_value:                    163
kern  :info  : [  358.634931] dphy_link_en_fuses:           0
kern  :info  : [  358.634931] lanes_clock_division:         0
kern  :info  : [  358.634932] link_used:                    0
kern  :info  : [  358.634932] lanes_used:                   2
kern  :info  : [  358.634933] csi_rx_dly_cnt_termen_clane:  0
kern  :info  : [  358.634933] csi_rx_dly_cnt_settle_clane:  0
kern  :info  : [  358.634934] csi_rx_dly_cnt_termen_dlane0: 0
kern  :info  : [  358.634934] csi_rx_dly_cnt_settle_dlane0: 0
kern  :info  : [  358.634935] csi_rx_dly_cnt_termen_dlane1: 0
kern  :info  : [  358.634936] csi_rx_dly_cnt_settle_dlane1: 0
kern  :info  : [  358.634936] csi_rx_dly_cnt_termen_dlane2: 0
kern  :info  : [  358.634937] csi_rx_dly_cnt_settle_dlane2: 0
kern  :info  : [  358.634938] csi_rx_dly_cnt_termen_dlane3: 0
kern  :info  : [  358.634938] csi_rx_dly_cnt_settle_dlane3: 0
kern  :info  : [  358.634939] max_lane_speed:               0
kern  :info  : [  358.634940] sensor_cal_file_idx:          0
kern  :info  : [  358.634940] sensor_cal_file_idx_mbz:
kern  :info  : [  358.634941] 00000000: 00 00 00                                         ...
kern  :info  : [  358.634942] rom_type:                     1
kern  :info  : [  358.634943] vcm_type:                     4
kern  :info  : [  358.634944] platform:                     9
kern  :info  : [  358.634944] platform_sub:                 0
kern  :info  : [  358.634945] flash_support:                2
kern  :info  : [  358.634945] privacy_led:                  0
kern  :info  : [  358.634946] degree:                       180
kern  :info  : [  358.634946] mipi_define:                  1
kern  :info  : [  358.634947] mclk_speed:                   19200000
kern  :info  : [  358.634948] control_logic_id:             0
kern  :info  : [  358.634949] mipi_data_format:             0
kern  :info  : [  358.634949] silicon_version:              0
kern  :info  : [  358.634950] customer_id:                  0
kern  :info  : [  358.634951] mclk_port:                    0
kern  :info  : [  358.634951] reserved:
kern  :info  : [  358.634952] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............
kern  :info  : [  358.635073] dump_i2c_dev_dsm(): i2c device amount: 2
kern  :info  : [  358.635258] dump_i2c_dev_dsm(): i2c device _DSM data (1 of 2): 0x02003600
kern  :info  : [  358.635445] dump_i2c_dev_dsm(): i2c device _DSM data (2 of 2): 0x02000c01

kern  :info  : [  358.635636] acpi INT3472:00: -------------------- INT3472:00 --------------------
kern  :info  : [  358.635709] ACPI path: \_SB_.PCI0.DSC0
kern  :info  : [  358.635710] ACPI device name: INT3472:00
kern  :info  : [  358.635715] print_i2c_dev_name(): i2c device not found. Possible reasons:
kern  :info  : [  358.635715] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
kern  :info  : [  358.635716] print_i2c_dev_name(): 2) It's a sensor device and the real sensor driver is loaded with the bridge driver.
kern  :info  : [  358.635717] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
kern  :info  : [  358.635719] print_dep_acpi_paths(): No _DEP entry found
kern  :info  : [  358.635720] acpi INT3472:00: ========== dump_crs() ==========
kern  :info  : [  358.636054] full raw output of _CRS:
kern  :info  : [  358.636057] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [  358.636057] 00000010: 00 19 00 28 00 00 00 7e 00 5c 5f 53 42 2e 50 43  ...(...~.\_SB.PC
kern  :info  : [  358.636058] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
kern  :info  : [  358.636059] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 7f  ...........(....
kern  :info  : [  358.636059] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
kern  :info  : [  358.636060] 00000050: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [  358.636061] 00000060: 00 19 00 28 00 00 00 8f 00 5c 5f 53 42 2e 50 43  ...(.....\_SB.PC
kern  :info  : [  358.636061] 00000070: 49 30 2e 47 50 49 30 00 79 00                    I0.GPI0.y.
kern  :info  : [  358.636064] acpi INT3472:00: ========== dump_cldb() ==========
kern  :info  : [  358.636064] full raw output of CLDB:
kern  :info  : [  358.636065] 00000000: 00 01 00 30 00 00 00 00 00 00 00 00 00 00 00 00  ...0............
kern  :info  : [  358.636065] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  358.636066] version:            0
kern  :info  : [  358.636066] control_logic_type: 1
kern  :info  : [  358.636067] control_logic_id:   0
kern  :info  : [  358.636067] sensor_card_sku:    48
kern  :info  : [  358.636068] reserved:
kern  :info  : [  358.636068] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  358.636069] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............
kern  :info  : [  358.636070] print_pmic_type(): PMIC type is "1: DISCRETE(CRD-D)"
kern  :info  : [  358.636153] __dump_pmic_dsm_discrete(): GPIO pin amount: 3
kern  :info  : [  358.636306] __dump_pmic_dsm_discrete(): GPIO pin _DSM data (1 of 3): 0x01007e00
kern  :info  : [  358.636474] __dump_pmic_dsm_discrete(): GPIO pin _DSM data (2 of 3): 0x01007f0b
kern  :info  : [  358.637741] __dump_pmic_dsm_discrete(): GPIO pin _DSM data (3 of 3): 0x01008f0c
```
