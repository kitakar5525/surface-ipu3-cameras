/**
 * This tool parses the SSDB fields and dumps many of the fields.
 */

#include <stdio.h>
#include <stdint.h>
#include "ssdb_dump.h"

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
	printf("SB2 CAMF_SKC1 CLDB data:\n");
	dump_cldb(sb2_camf_skc1_cldb);
	printf("SB2 CAM3_SKC2 CLDB data:\n");
	dump_cldb(sb2_cam3_skc2_cldb);

	/* SB1 SSDB */
	printf("SB1 CAMR SSDB data:\n");
	dump_ssdb(sb1_camr_ssdb);
	printf("SB1 CAMF SSDB data:\n");
	dump_ssdb(sb1_camf_ssdb);
	printf("SB1 CAM3 SSDB data:\n");
	dump_ssdb(sb1_cam3_ssdb);

	/* SB1 CLDB */
	printf("SB1 CAMR_SKC0 CLDB data:\n");
	dump_cldb(sb1_camr_skc0_cldb);
	printf("SB1 CAMF_SKC1 CLDB data:\n");
	dump_cldb(sb1_camf_skc1_cldb);
	printf("SB1 CAM3_SKC2 CLDB data:\n");
	dump_cldb(sb1_cam3_skc2_cldb);

	/* SGO2 SSDB */
	printf("SGO2 LNK0 SSDB data\n");
	dump_ssdb(sgo2_lnk0_ssdb);
	printf("SGO2 LNK1 SSDB data\n");
	dump_ssdb(sgo2_lnk1_ssdb);
	printf("SGO2 LNK2 SSDB data\n");
	dump_ssdb(sgo2_lnk2_ssdb);

	/* SGO2 CLDB */
	printf("SGO2 LNK0/LNK2 CLP0 CLDB data\n");
	dump_cldb(sgo2_lnk0_lnk2_clp0_cldb);
	printf("SGO2 LNK1 DSC1 CLDB data\n");
	dump_cldb(sgo2_lnk1_dsc1_cldb);

	return 0;
}

