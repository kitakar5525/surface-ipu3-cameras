/* SPDX-License-Identifier: GPL-2.0 */

#define DRV_NAME "get_acpi_data"

/* From coreboot */
struct intel_ssdb {
	uint8_t version;			/* Current version */
	uint8_t sensor_card_sku;		/* CRD Board type */
	uint8_t csi2_data_stream_interface[16];	/* CSI2 data stream GUID */
	uint16_t bdf_value;			/* Bus number of the host
						 * controller
						 */
	uint32_t dphy_link_en_fuses;		/* Host controller's fuses
						 * information used to verify if
						 * link is fused out or not
						 */
	uint32_t lanes_clock_division;		/* Lanes/clock divisions per
						 * sensor
						 */
	uint8_t link_used;			/* Link used by this sensor
						 * stream
						 */
	uint8_t lanes_used;			/* Number of lanes connected for
						 * the sensor
						 */
	uint32_t csi_rx_dly_cnt_termen_clane;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_settle_clane;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_termen_dlane0;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_settle_dlane0;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_termen_dlane1;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_settle_dlane1;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_termen_dlane2;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_settle_dlane2;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_termen_dlane3;	/* MIPI timing information */
	uint32_t csi_rx_dly_cnt_settle_dlane3;	/* MIPI timing information */
	uint32_t max_lane_speed;		/* Maximum lane speed for
						 * the sensor
						 */
	uint8_t sensor_cal_file_idx;		/* Legacy field for sensor
						 * calibration file index
						 */
	uint8_t sensor_cal_file_idx_mbz[3];	/* Legacy field for sensor
						 * calibration file index
						 */
	uint8_t rom_type;			/* NVM type of the camera
						 * module
						 */
	uint8_t vcm_type;			/* VCM type of the camera
						 * module
						 */
	uint8_t platform;			/* Platform information */
	uint8_t platform_sub;			/* Platform sub-categories */
	uint8_t flash_support;			/* Enable/disable flash
						 * support
						 */
	uint8_t privacy_led;			/* Privacy LED support */
	uint8_t degree;				/* Camera Orientation */
	uint8_t mipi_define;			/* MIPI info defined in ACPI or
						 * sensor driver
						 */
	uint32_t mclk_speed;			/* Clock info for sensor */
	uint8_t control_logic_id;		/* PMIC device node used for
						 * the camera sensor
						 */
	uint8_t mipi_data_format;		/* MIPI data format */
	uint8_t silicon_version;		/* Silicon version */
	uint8_t customer_id;			/* Customer ID */
	uint8_t mclk_port;
	uint8_t reserved[13];			/* Pads SSDB out so the binary blob in ACPI is
						 * the same size as seen on other firmwares.
						 */
} __attribute__((__packed__));

/* From old chromiumos' ACPI info reading implementation */
struct intel_cldb {
	u8 version;
	/*
	 * control logic type
	 * 0: UNKNOWN
	 * 1: DISCRETE(CRD-D)
	 * 2: PMIC TPS68470
	 * 3: PMIC uP6641
	 */
	u8 control_logic_type;
	u8 control_logic_id; /* PMIC device node used for the camera sensor */
	u8 sensor_card_sku;
	u8 reserved[28];
} __attribute__((__packed__));

struct ipu3_sensor {
	const char acpi_hid[20];
};
