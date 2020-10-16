#### result on Acer Switch Alpha 12

Result with tree https://github.com/kitakar5525/surface-ipu3-cameras/tree/fe3ff654fb0e20563521300a26901f831be4ead5

```
kern  :info  : [  283.876705] acpi INT3479:00: -------------------- INT3479:00 --------------------
kern  :info  : [  283.877762] Sensor name: ov5670
kern  :info  : [  283.877767] ACPI path: \_SB_.PCI0.LNK0
kern  :info  : [  283.877769] ACPI device name: INT3479:00
kern  :info  : [  283.877779] i2c device name: i2c-INT3479:00
kern  :info  : [  283.878254] _DEP ACPI path (1 of 2): \_SB_.PCI0.DSC0
kern  :info  : [  283.878257] _DEP ACPI path (2 of 2): \_SB_.PCI0.I2C2
kern  :info  : [  283.878261] acpi INT3479:00: ========== dump_crs() ==========
kern  :info  : [  283.879318] full raw output of _CRS:
kern  :info  : [  283.879322] 00000000: 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06 00  ................
kern  :info  : [  283.879325] 00000010: 36 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43 32  6.\_SB.PCI0.I2C2
kern  :info  : [  283.879327] 00000020: 00 8e 1e 00 01 00 01 02 00 00 01 06 00 80 1a 06  ................
kern  :info  : [  283.879329] 00000030: 00 0c 00 5c 5f 53 42 2e 50 43 49 30 2e 49 32 43  ...\_SB.PCI0.I2C
kern  :info  : [  283.879332] 00000040: 32 00 79 00                                      2.y.
kern  :info  : [  283.879335] acpi INT3479:00: ========== dump_ssdb() ==========
kern  :info  : [  283.879336] full raw output of SSDB:
kern  :info  : [  283.879339] 00000000: 00 30 69 56 39 8a f7 11 a9 4e 9c 7d 20 ee 0a b5  .0iV9....N.} ...
kern  :info  : [  283.879341] 00000010: ca 40 a3 00 00 00 00 00 00 00 00 00 00 02 00 00  .@..............
kern  :info  : [  283.879343] 00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  283.879345] 00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  283.879347] 00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 01 04  ................
kern  :info  : [  283.879349] 00000050: 09 00 02 00 b4 01 00 f8 24 01 00 00 00 00 00 00  ........$.......
kern  :info  : [  283.879351] 00000060: 00 00 00 00 00 00 00 00 00 00 00 00              ............
kern  :info  : [  283.879352] ======= Important data =======
kern  :info  : [  283.879354] link_used:     0
kern  :info  : [  283.879391] lanes_used:    2
kern  :info  : [  283.879393] vcm_type:      4
kern  :info  : [  283.879394] flash_support: 2
kern  :info  : [  283.879397] degree:        180
kern  :info  : [  283.879404] mclk_speed:    19200000
kern  :info  : [  283.879414] mclk_port:     0
kern  :info  : [  283.879415] ========== All data ==========
kern  :info  : [  283.879417] version:                      0
kern  :info  : [  283.879419] sensor_card_sku:              48
kern  :info  : [  283.879420] csi2_data_stream_interface:
kern  :info  : [  283.879424] 00000000: 69 56 39 8a f7 11 a9 4e 9c 7d 20 ee 0a b5 ca 40  iV9....N.} ....@
kern  :info  : [  283.879426] bdf_value:                    163
kern  :info  : [  283.879428] dphy_link_en_fuses:           0
kern  :info  : [  283.879430] lanes_clock_division:         0
kern  :info  : [  283.879431] link_used:                    0
kern  :info  : [  283.879433] lanes_used:                   2
kern  :info  : [  283.879434] csi_rx_dly_cnt_termen_clane:  0
kern  :info  : [  283.879436] csi_rx_dly_cnt_settle_clane:  0
kern  :info  : [  283.879443] csi_rx_dly_cnt_termen_dlane0: 0
kern  :info  : [  283.879444] csi_rx_dly_cnt_settle_dlane0: 0
kern  :info  : [  283.879446] csi_rx_dly_cnt_termen_dlane1: 0
kern  :info  : [  283.879448] csi_rx_dly_cnt_settle_dlane1: 0
kern  :info  : [  283.879449] csi_rx_dly_cnt_termen_dlane2: 0
kern  :info  : [  283.879451] csi_rx_dly_cnt_settle_dlane2: 0
kern  :info  : [  283.879453] csi_rx_dly_cnt_termen_dlane3: 0
kern  :info  : [  283.879454] csi_rx_dly_cnt_settle_dlane3: 0
kern  :info  : [  283.879456] max_lane_speed:               0
kern  :info  : [  283.879458] sensor_cal_file_idx:          0
kern  :info  : [  283.879460] sensor_cal_file_idx_mbz:
kern  :info  : [  283.879463] 00000000: 00 00 00                                         ...
kern  :info  : [  283.879465] rom_type:                     1
kern  :info  : [  283.879467] vcm_type:                     4
kern  :info  : [  283.879468] platform:                     9
kern  :info  : [  283.879470] platform_sub:                 0
kern  :info  : [  283.879472] flash_support:                2
kern  :info  : [  283.879473] privacy_led:                  0
kern  :info  : [  283.879475] degree:                       180
kern  :info  : [  283.879477] mipi_define:                  1
kern  :info  : [  283.879479] mclk_speed:                   19200000
kern  :info  : [  283.879481] control_logic_id:             0
kern  :info  : [  283.879482] mipi_data_format:             0
kern  :info  : [  283.879484] silicon_version:              0
kern  :info  : [  283.879485] customer_id:                  0
kern  :info  : [  283.879487] mclk_port:                    0
kern  :info  : [  283.879488] reserved:
kern  :info  : [  283.879493] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00           .............

kern  :info  : [  283.880100] acpi INT3472:00: -------------------- INT3472:00 --------------------
kern  :info  : [  283.880334] ACPI path: \_SB_.PCI0.DSC0
kern  :info  : [  283.880337] ACPI device name: INT3472:00
kern  :info  : [  283.880345] print_i2c_dev_name(): i2c device not found. Possible reasons:
kern  :info  : [  283.880347] print_i2c_dev_name(): 1) It's a PMIC and the PMIC type is DISCRETE.
kern  :info  : [  283.880349] print_i2c_dev_name(): 2) It's a sensor driver and the real sensor driver is loaded with the bridge driver.
kern  :info  : [  283.880351] print_i2c_dev_name(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.
kern  :info  : [  283.880357] print_dep_acpi_paths(): No _DEP entry found
kern  :info  : [  283.880362] acpi INT3472:00: ========== dump_crs() ==========
kern  :info  : [  283.881591] full raw output of _CRS:
kern  :info  : [  283.881596] 00000000: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [  283.881603] 00000010: 00 19 00 28 00 00 00 7e 00 5c 5f 53 42 2e 50 43  ...(...~.\_SB.PC
kern  :info  : [  283.881606] 00000020: 49 30 2e 47 50 49 30 00 8c 25 00 01 01 01 00 02  I0.GPI0..%......
kern  :info  : [  283.881609] 00000030: 00 00 00 00 00 00 17 00 00 19 00 28 00 00 00 7f  ...........(....
kern  :info  : [  283.881613] 00000040: 00 5c 5f 53 42 2e 50 43 49 30 2e 47 50 49 30 00  .\_SB.PCI0.GPI0.
kern  :info  : [  283.881617] 00000050: 8c 25 00 01 01 01 00 02 00 00 00 00 00 00 17 00  .%..............
kern  :info  : [  283.881620] 00000060: 00 19 00 28 00 00 00 8f 00 5c 5f 53 42 2e 50 43  ...(.....\_SB.PC
kern  :info  : [  283.881623] 00000070: 49 30 2e 47 50 49 30 00 79 00                    I0.GPI0.y.
kern  :info  : [  283.881629] acpi INT3472:00: ========== dump_cldb() ==========
kern  :info  : [  283.881630] full raw output of CLDB:
kern  :info  : [  283.881633] 00000000: 00 01 00 30 00 00 00 00 00 00 00 00 00 00 00 00  ...0............
kern  :info  : [  283.881636] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  283.881639] version:            0
kern  :info  : [  283.881641] control_logic_type: 1
kern  :info  : [  283.881643] control_logic_id:   0
kern  :info  : [  283.881645] sensor_card_sku:    48
kern  :info  : [  283.881647] reserved:
kern  :info  : [  283.881651] 00000000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
kern  :info  : [  283.881657] 00000010: 00 00 00 00 00 00 00 00 00 00 00 00              ............
```
