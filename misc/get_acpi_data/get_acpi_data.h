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
	const char sensor_name[20];
};

/*
 * _DSM that returns subsystem ID (found in sensor devices)
 */
#define SUBSYS_ID_DSM_REV			0x0
#define SUBSYS_ID_DSM_RETURN_ID_FUNC		0x1
/* SUBSYSTEM ID _DSM UUID: "822ace8f-2814-4174-a56b-5f029fe079ee" */
static const guid_t subsys_id_dsm_guid =
	GUID_INIT(0x822ace8f, 0x2814, 0x4174,
		  0xa5, 0x6b, 0x5f, 0x02, 0x9f, 0xe0, 0x79, 0xee);

/*
 * _DSM that returns i2c dev data (found in sensors and non-discrete PMICs)
 */
#define I2C_DEV_DSM_REV			0x0
/* The amount of devices defined in _CRS */
#define I2C_DEV_DSM_DEV_AMOUNT_FUNC	0x1
/* For func, after 0x2 is the funcs for getting i2c dev data. There are
 * as many of funcs as the amount of i2c devices.
 */
/* I2C_DEV _DSM UUID: "26257549-9271-4ca4-bb43-c4899d5a4881" */
static const guid_t i2c_dev_dsm_guid =
	GUID_INIT(0x26257549, 0x9271, 0x4ca4,
		  0xbb, 0x43, 0xc4, 0x89, 0x9d, 0x5a, 0x48, 0x81);

/*
 * _DSM that returns GPIO pin data (for discrete PMICs)
 */
#define DISCRETE_PMIC_DSM_REV			0x0
/* The amount of devices defined in _CRS */
#define DISCRETE_PMIC_DSM_GPIO_AMOUNT_FUNC	0x1
/* For func, after 0x2 is the funcs for getting GPIO pin data. There are
 * as many of funcs as the amount of GPIO pins.
 */
/* DISCRETE PMIC _DSM UUID: "79234640-9e10-4fea-a5c1-b5aa8b19756f" */
static const guid_t pmic_dsm_guid =
	GUID_INIT(0x79234640, 0x9e10, 0x4fea,
		  0xa5, 0xc1, 0xb5, 0xaa, 0x8b, 0x19, 0x75, 0x6f);

/*
 * _DSM UUID found on some devices (Surface Go 1 / Go 2, Acer Switch
 * Alpha 12 ...) (found in sensors and non-discrete PMICs)
 */
#define DSMB_DSM_REV			0x0
/* This func returns a buffer called DSMB, but the details are currently
 * not known.
 */
#define DSMB_DSM_RETURN_BUF_FUNC	0x1
/* DSMB _DSM UUID: "5815c5c8-c47d-477b-9a8d-76173176414b" */
static const guid_t dsmb_dsm_guid =
	GUID_INIT(0x5815c5c8, 0xc47d, 0x477b,
		  0x9a, 0x8d, 0x76, 0x17, 0x31, 0x76, 0x41, 0x4b);
