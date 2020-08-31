/**
 * This tool parses the SSDB fields and dumps many of the fields.
 */

#include <stdio.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

// These are hardcoded values for the ssdb dumped from the Surface Book 2
// DSDT table.
uint8_t sb2_camr_ssdb[] = {
	/* 0000 */  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // . ......
	/* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,  // ........
	/* 0020 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0028 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0030 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0038 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0040 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0048 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04,  // ........
	/* 0050 */  0x09, 0x00, 0x02, 0x01, 0x01, 0x01, 0x00, 0xF8,  // ........
	/* 0058 */  0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // $.......
	/* 0060 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0068 */  0x00, 0x00, 0x00, 0x00                           // ....
};
uint8_t sb2_camf_ssdb[] = {
	/* 0000 */  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // . ......
	/* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0018 */  0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,  // ........
	/* 0020 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0028 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0030 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0038 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0040 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0048 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,  // ........
	/* 0050 */  0x09, 0x00, 0x02, 0x01, 0x01, 0x01, 0x00, 0xF8,  // ........
	/* 0058 */  0x24, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // $.......
	/* 0060 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0068 */  0x00, 0x00, 0x00, 0x00                           // ....
};
uint8_t sb2_cam3_ssdb[] = {
	/* 0000 */  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // . ......
	/* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0018 */  0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00,  // ........
	/* 0020 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0028 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0030 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0038 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0040 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0048 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0050 */  0x09, 0x00, 0x02, 0x00, 0x00, 0x01, 0x00, 0xF8,  // ........
	/* 0058 */  0x24, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,  // $.......
	/* 0060 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0068 */  0x00, 0x00, 0x00, 0x00                           // ....
};
// These are hardcoded values for the cldb dumped from the Surface Book 2
// DSDT table.
uint8_t sb2_camr_skc0_cldb[] = {
	/* 0000 */  0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,  // ... ....
	/* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // ........
};
uint8_t sb2_camf_skc1_cldb[] = {
	/* 0000 */  0x00, 0x01, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00,  // ... ....
	/* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // ........
};
uint8_t sb2_cam3_skc2_cldb[] = {
	/* 0000 */  0x00, 0x01, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,  // ... ....
	/* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
	/* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // ........
};

/* From Intel's ipu4-acpi */
struct sensor_bios_data_packed {
	u8 version;
	u8 sku;
	u8 guid_csi2[16];
	u8 devfunction;
	u8 bus;
	u32 dphylinkenfuses;
	u32 clockdiv;
	u8 link;
	u8 lanes;
	u32 csiparams[10];
	u32 maxlanespeed;
	u8 sensorcalibfileidx;
	u8 sensorcalibfileidxInMBZ[3];
	u8 romtype;
	u8 vcmtype;
	u8 platforminfo;
	u8 platformsubinfo;
	u8 flash;
	u8 privacyled;
	u8 degree;
	u8 mipilinkdefined;
	u32 mclkspeed;
	u8 controllogicid;
	u8 reserved1[3];
	u8 mclkport;
	u8 reserved2[13];
} __attribute__((__packed__));

/* From coreboot */
struct intel_ssdb {
	uint8_t version;			/* Current version */
	uint8_t sensor_card_sku;		/* CRD Board type */
	uint8_t csi2_data_stream_interface[16];	/* CSI2 data stream GUID */
	uint16_t bdf_value;			/* Bus number of the host
						controller */
	uint32_t dphy_link_en_fuses;		/* Host controller's fuses
						information used to verify if
						link is fused out or not */
	uint32_t lanes_clock_division;		/* Lanes/clock divisions per
						sensor */
	uint8_t link_used;			/* Link used by this sensor
						stream */
	uint8_t lanes_used;			/* Number of lanes connected for
						the sensor */
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
						the sensor */
	uint8_t sensor_cal_file_idx;		/* Legacy field for sensor
						calibration file index */
	uint8_t sensor_cal_file_idx_mbz[3];	/* Legacy field for sensor
						calibration file index */
	uint8_t rom_type;			/* NVM type of the camera
						module */
	uint8_t vcm_type;			/* VCM type of the camera
						module */
	uint8_t platform;			/* Platform information */
	uint8_t platform_sub;			/* Platform sub-categories */
	uint8_t flash_support;			/* Enable/disable flash
						support */
	uint8_t privacy_led;			/* Privacy LED support */
	uint8_t degree;				/* Camera Orientation */
	uint8_t mipi_define;			/* MIPI info defined in ACPI or
						sensor driver */
	uint32_t mclk_speed;			/* Clock info for sensor */
	uint8_t control_logic_id;		/* PMIC device node used for
						the camera sensor */
	uint8_t mipi_data_format;		/* MIPI data format */
	uint8_t silicon_version;		/* Silicon version */
	uint8_t customer_id;			/* Customer ID */
	uint8_t mclk_port;
	uint8_t reserved[13];			/* Pads SSDB out so the binary blob in ACPI is
						   the same size as seen on other firmwares.*/
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

void dump_ssdb(uint8_t *data) {
	struct intel_ssdb *d =
		(struct intel_ssdb *)data;

	printf("lanes_clock_division: %d\n", d->lanes_clock_division);
	printf("link_used: %d\n", d->link_used);
	printf("lanes_used: %d\n", d->lanes_used);
	printf("mclk_speed: %d\n", d->mclk_speed);
	printf("mclk_port: %d\n", d->mclk_port);

	printf("/* Additional data */\n");
	printf("rom_type: %d\n", d->rom_type);
	printf("vcm_type: %d\n", d->vcm_type);
	printf("flash_support: %d\n", d->flash_support);
	printf("privacy_led: %d\n", d->privacy_led);
	printf("degree: %d\n", d->degree);
	printf("mipi_define: %d\n", d->mipi_define);
	printf("control_logic_id: %d\n", d->control_logic_id);

	printf("\n");
}

void dump_cldb(uint8_t *data) {
	struct intel_cldb *d =
		(struct intel_cldb *)data;

	printf("version: %d\n", d->version);
	printf("control_logic_type: %d\n", d->control_logic_type);
	printf("control_logic_id: %d\n", d->control_logic_id);
	printf("sensor_card_sku: %d\n", d->sensor_card_sku);
	printf("\n");
}

int main() {
	/* SB2 SSDB */
	printf("SB2 CAMR SSDB data:\n");
	dump_ssdb(sb2_camr_ssdb);
	printf("SB2 CAMF SSDB data:\n");
	dump_ssdb(sb2_camf_ssdb);
	printf("SB2 CAM3 SSDB data:\n");
	dump_ssdb(sb2_cam3_ssdb);

	/* SB2 CLDB */
	printf("SB2 CAMR_SKC0 CLDB data:\n");
	dump_cldb(sb2_camr_skc0_cldb);
	printf("SB2 CAMF_SKC2 CLDB data:\n");
	dump_cldb(sb2_camf_skc1_cldb);
	printf("SB2 CAM3_SKC2 CLDB data:\n");
	dump_cldb(sb2_cam3_skc2_cldb);

	return 0;
}

