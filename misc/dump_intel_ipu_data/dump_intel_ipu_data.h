/* SPDX-License-Identifier: GPL-2.0 */

struct device_count {
	int sensor;
	int pmic;
};

/* From coreboot */
struct intel_ssdb {
	u8 version;				/* Current version */
	u8 sensor_card_sku;			/* CRD Board type */
	u8 csi2_data_stream_interface[16];	/* CSI2 data stream GUID */
	u16 bdf_value;				/* Bus number of the host
						 * controller
						 */
	u32 dphy_link_en_fuses;			/* Host controller's fuses
						 * information used to verify
						 * if link is fused out or not
						 */
	u32 lanes_clock_division;		/* Lanes/clock divisions per
						 * sensor
						 */
	u8 link_used;				/* Link used by this sensor
						 * stream
						 */
	u8 lanes_used;				/* Number of lanes connected
						 * for the sensor
						 */
	u32 csi_rx_dly_cnt_termen_clane;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_settle_clane;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_termen_dlane0;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_settle_dlane0;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_termen_dlane1;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_settle_dlane1;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_termen_dlane2;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_settle_dlane2;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_termen_dlane3;	/* MIPI timing information */
	u32 csi_rx_dly_cnt_settle_dlane3;	/* MIPI timing information */
	u32 max_lane_speed;			/* Maximum lane speed for
						 * the sensor
						 */
	u8 sensor_cal_file_idx;			/* Legacy field for sensor
						 * calibration file index
						 */
	u8 sensor_cal_file_idx_mbz[3];		/* Legacy field for sensor
						 * calibration file index
						 */
	u8 rom_type;				/* NVM type of the camera
						 * module
						 */
	u8 vcm_type;				/* VCM type of the camera
						 * module
						 */
	u8 platform;				/* Platform information */
	u8 platform_sub;			/* Platform sub-categories */
	u8 flash_support;			/* Enable/disable flash
						 * support
						 */
	u8 privacy_led;				/* Privacy LED support */
	u8 degree;				/* Camera Orientation */
	u8 mipi_define;				/* MIPI info defined in ACPI or
						 * sensor driver
						 */
	u32 mclk_speed;				/* Clock info for sensor */
	u8 control_logic_id;			/* PMIC device node used for
						 * the camera sensor
						 */
	u8 mipi_data_format;			/* MIPI data format */
	u8 silicon_version;			/* Silicon version */
	u8 customer_id;				/* Customer ID */
	u8 mclk_port;
	u8 reserved[13];			/* Pads SSDB out so the binary
						 * blob in ACPI is the same
						 * size as seen on other
						 * firmwares.
						 */
} __packed;

/* From old chromiumos ACPI data reading implementation */
struct intel_cldb {
	u8 version;
	/* control logic type
	 * 0: UNKNOWN
	 * 1: DISCRETE(CRD-D)
	 * 2: PMIC TPS68470
	 * 3: PMIC uP6641
	 */
	u8 control_logic_type;
	u8 control_logic_id; /* PMIC device node used for the camera sensor */
	u8 sensor_card_sku;
	u8 reserved[28];
} __packed;

enum control_logic_type {
	PMIC_TYPE_UNKNOWN,
	PMIC_TYPE_DISCRETE,
	PMIC_TYPE_TPS68470,
	PMIC_TYPE_UP6641,
};

const char *control_logic_type_list[] = {
	"0: UNKNOWN",
	"1: DISCRETE(CRD-D)",
	"2: PMIC TPS68470",
	"3: PMIC uP6641",
};

/*
 * PLD (Physical Device Location) int to string conversion.
 * From drivers/acpi/acpica/utglobal.c
 */
const char *pld_panel_list[] = {
	"TOP",
	"BOTTOM",
	"LEFT",
	"RIGHT",
	"FRONT",
	"BACK",
	"UNKNOWN",
	NULL
};

const char *pld_vertical_position_list[] = {
	"UPPER",
	"CENTER",
	"LOWER",
	NULL
};

const char *pld_horizontal_position_list[] = {
	"LEFT",
	"CENTER",
	"RIGHT",
	NULL
};

const char *pld_shape_list[] = {
	"ROUND",
	"OVAL",
	"SQUARE",
	"VERTICALRECTANGLE",
	"HORIZONTALRECTANGLE",
	"VERTICALTRAPEZOID",
	"HORIZONTALTRAPEZOID",
	"UNKNOWN",
	"CHAMFERED",
	NULL
};

#define DSM_STR_BUF_SIZE 32

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
/* 0x2 and later is the funcs for getting i2c dev data. There are
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
/* 0x2 and later is the funcs for getting GPIO pin data. There are
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
/* This func returns a buffer called DSMB */
#define DSMB_DSM_RETURN_BUF_FUNC	0x1
/* DSMB _DSM UUID: "5815c5c8-c47d-477b-9a8d-76173176414b" */
static const guid_t dsmb_dsm_guid =
	GUID_INIT(0x5815c5c8, 0xc47d, 0x477b,
		  0x9a, 0x8d, 0x76, 0x17, 0x31, 0x76, 0x41, 0x4b);
