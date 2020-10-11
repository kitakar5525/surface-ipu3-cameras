/**
 * This tool parses the SSDB fields and dumps many of the fields.
 */

#include <stdio.h>
#include <stdint.h>
#include "ssdb_dump.h"

void dump_ssdb(uint8_t *data) {
	struct intel_ssdb *d =
		(struct intel_ssdb *)data;

	printf("========== %s() ==========\n", __func__);

	printf("lanes_clock_division: %d\n", d->lanes_clock_division);
	printf("link_used: %d\n", d->link_used);
	printf("lanes_used: %d\n", d->lanes_used);
	printf("vcm_type: %d\n", d->vcm_type);
	printf("flash_support: %d\n", d->flash_support);
	printf("degree: %d\n", d->degree);
	printf("mclk_port: %d\n", d->mclk_port);
	printf("mclk_speed: %d\n", d->mclk_speed);

	printf("/* Additional data */\n");
	printf("lanes_clock_division: %d\n", d->lanes_clock_division);
	printf("rom_type: %d\n", d->rom_type);
	printf("privacy_led: %d\n", d->privacy_led);
	printf("mipi_define: %d\n", d->mipi_define);
	printf("control_logic_id: %d\n", d->control_logic_id);

	printf("\n");
}

void dump_cldb(uint8_t *data) {
	struct intel_cldb *d =
		(struct intel_cldb *)data;

	printf("========== %s() ==========\n", __func__);

	printf("version: %d\n", d->version);
	printf("control_logic_type: %d\n", d->control_logic_type);
	printf("control_logic_id: %d\n", d->control_logic_id);
	printf("sensor_card_sku: %d\n", d->sensor_card_sku);

	printf("\n");
}

int main() {
	/* SB2 */
	printf("-------------------- SB2 CAMR --------------------\n");
	dump_ssdb(sb2_camr_ssdb);
	dump_cldb(sb2_camr_skc0_cldb);
	printf("-------------------- SB2 CAMF --------------------\n");
	dump_ssdb(sb2_camf_ssdb);
	dump_cldb(sb2_camf_skc1_cldb);
	printf("-------------------- SB2 CAM3 --------------------\n");
	dump_ssdb(sb2_cam3_ssdb);
	dump_cldb(sb2_cam3_skc2_cldb);

	/* SB1 */
	printf("-------------------- SB1 CAMR --------------------\n");
	dump_ssdb(sb1_camr_ssdb);
	dump_cldb(sb1_camr_skc0_cldb);
	printf("-------------------- SB1 CAMF --------------------\n");
	dump_ssdb(sb1_camf_ssdb);
	dump_cldb(sb1_camf_skc1_cldb);
	printf("-------------------- SB1 CAM3 --------------------\n");
	dump_ssdb(sb1_cam3_ssdb);
	dump_cldb(sb1_cam3_skc2_cldb);

	/* SGO2 */
	printf("-------------------- SGO2 LNK0 --------------------\n");
	dump_ssdb(sgo2_lnk0_ssdb);
	printf("-------------------- SGO2 LNK1 --------------------\n");
	dump_ssdb(sgo2_lnk1_ssdb);
	printf("-------------------- SGO2 LNK2 --------------------\n");
	dump_ssdb(sgo2_lnk2_ssdb);
	printf("-------------------- SGO2 LNK0/LNK2 CLDB --------------------\n");
	dump_cldb(sgo2_lnk0_lnk2_clp0_cldb);
	printf("-------------------- SGO2 LNK1 CLDB --------------------\n");
	dump_cldb(sgo2_lnk1_dsc1_cldb);

	return 0;
}
