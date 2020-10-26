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
