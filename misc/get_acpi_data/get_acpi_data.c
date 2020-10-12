// SPDX-License-Identifier: GPL-2.0

#include <linux/acpi.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>

#include "get_acpi_data.h"

static void dump_ssdb(struct device *dev, struct intel_ssdb *data, int data_len)
{
	dev_info(dev, "========== %s() ==========\n", __func__);

	pr_info("full raw output of SSDB:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       data, data_len, true);

	pr_info("======= Important data =======\n");
	pr_info("link_used:     %d\n", data->link_used);
	pr_info("lanes_used:    %d\n", data->lanes_used);
	pr_info("vcm_type:      %d\n", data->vcm_type);
	pr_info("flash_support: %d\n", data->flash_support);
	pr_info("degree:        %d\n", data->degree);
	pr_info("mclk_speed:    %d\n", data->mclk_speed);
	pr_info("mclk_port:     %d\n", data->mclk_port);

	pr_info("========== All data ==========\n");
	pr_info("version:                      %d\n", data->version);
	pr_info("sensor_card_sku:              %d\n", data->sensor_card_sku);
	pr_info("csi2_data_stream_interface:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       data->csi2_data_stream_interface,
		       sizeof(data->csi2_data_stream_interface), true);
	pr_info("bdf_value:                    %d\n", data->bdf_value);
	pr_info("dphy_link_en_fuses:           %d\n", data->dphy_link_en_fuses);
	pr_info("lanes_clock_division:         %d\n", data->lanes_clock_division);
	pr_info("link_used:                    %d\n", data->link_used);
	pr_info("lanes_used:                   %d\n", data->lanes_used);
	pr_info("csi_rx_dly_cnt_termen_clane:  %d\n", data->csi_rx_dly_cnt_termen_clane);
	pr_info("csi_rx_dly_cnt_settle_clane:  %d\n", data->csi_rx_dly_cnt_settle_clane);
	pr_info("csi_rx_dly_cnt_termen_dlane0: %d\n", data->csi_rx_dly_cnt_termen_dlane0);
	pr_info("csi_rx_dly_cnt_settle_dlane0: %d\n", data->csi_rx_dly_cnt_settle_dlane0);
	pr_info("csi_rx_dly_cnt_termen_dlane1: %d\n", data->csi_rx_dly_cnt_termen_dlane1);
	pr_info("csi_rx_dly_cnt_settle_dlane1: %d\n", data->csi_rx_dly_cnt_settle_dlane1);
	pr_info("csi_rx_dly_cnt_termen_dlane2: %d\n", data->csi_rx_dly_cnt_termen_dlane2);
	pr_info("csi_rx_dly_cnt_settle_dlane2: %d\n", data->csi_rx_dly_cnt_settle_dlane2);
	pr_info("csi_rx_dly_cnt_termen_dlane3: %d\n", data->csi_rx_dly_cnt_termen_dlane3);
	pr_info("csi_rx_dly_cnt_settle_dlane3: %d\n", data->csi_rx_dly_cnt_settle_dlane3);
	pr_info("max_lane_speed:               %d\n", data->max_lane_speed);
	pr_info("sensor_cal_file_idx:          %d\n", data->sensor_cal_file_idx);
	pr_info("sensor_cal_file_idx_mbz:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       data->sensor_cal_file_idx_mbz,
		       sizeof(data->sensor_cal_file_idx_mbz), true);
	pr_info("rom_type:                     %d\n", data->rom_type);
	pr_info("vcm_type:                     %d\n", data->vcm_type);
	pr_info("platform:                     %d\n", data->platform);
	pr_info("platform_sub:                 %d\n", data->platform_sub);
	pr_info("flash_support:                %d\n", data->flash_support);
	pr_info("privacy_led:                  %d\n", data->privacy_led);
	pr_info("degree:                       %d\n", data->degree);
	pr_info("mipi_define:                  %d\n", data->mipi_define);
	pr_info("mclk_speed:                   %d\n", data->mclk_speed);
	pr_info("control_logic_id:             %d\n", data->control_logic_id);
	pr_info("mipi_data_format:             %d\n", data->mipi_data_format);
	pr_info("silicon_version:              %d\n", data->silicon_version);
	pr_info("customer_id:                  %d\n", data->customer_id);
	pr_info("mclk_port:                    %d\n", data->mclk_port);
	pr_info("reserved:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       data->reserved, sizeof(data->reserved), true);
}

static void dump_cldb(struct device *dev, struct intel_cldb *data, int data_len)
{
	dev_info(dev, "========== %s() ==========\n", __func__);

	pr_info("full raw output of CLDB:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       data, data_len, true);

	pr_info("version:            %d\n", data->version);
	pr_info("control_logic_type: %d\n", data->control_logic_type);
	pr_info("control_logic_id:   %d\n", data->control_logic_id);
	pr_info("sensor_card_sku:    %d\n", data->sensor_card_sku);
	pr_info("reserved:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       data->reserved, sizeof(data->reserved), true);
}

static int read_acpi_block(struct device *dev, char *id, void *data, u32 size)
{
	union acpi_object *obj;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	struct acpi_handle *dev_handle = ACPI_HANDLE(dev);
	int status;
	u32 buffer_length;

	status = acpi_evaluate_object(dev_handle, id, NULL, &buffer);
	if (!ACPI_SUCCESS(status))
		return -ENODEV;

	obj = (union acpi_object *)buffer.pointer;
	if (!obj || obj->type != ACPI_TYPE_BUFFER) {
		dev_err(dev, "Could't read acpi buffer\n");
		status = -ENODEV;
		goto err;
	}

	if (obj->buffer.length > size) {
		dev_err(dev, "Given buffer is too small\n");
		status = -ENODEV;
		goto err;
	}

	memcpy(data, obj->buffer.pointer, min(size, obj->buffer.length));
	buffer_length = obj->buffer.length;
	kfree(buffer.pointer);

	return buffer_length;
err:
	kfree(buffer.pointer);
	return status;
}

/* Get dependent INT3472 device */
static struct device *get_dep_dev(struct device *dev)
{
	struct acpi_handle *dev_handle = ACPI_HANDLE(dev);
	struct acpi_handle_list dep_devices;
	struct acpi_device *dep_adev;
	struct acpi_device_physical_node *dep_phys;
	int ret;
	int i;

	if (!acpi_has_method(dev_handle, "_DEP")) {
		dev_err(dev, "No dependent devices\n");
		return ERR_PTR(-ENODEV);
	}

	ret = acpi_evaluate_reference(dev_handle, "_DEP", NULL, &dep_devices);
	if (ACPI_FAILURE(ret)) {
		dev_err(dev, "Failed to evaluate _DEP.\n");
		return ERR_PTR(-ENODEV);
	}

	for (i = 0; i < dep_devices.count; i++) {
		struct acpi_device_info *info;

		ret = acpi_get_object_info(dep_devices.handles[i], &info);
		if (ACPI_FAILURE(ret)) {
			dev_err(dev, "Error reading _DEP device info\n");
			return ERR_PTR(-ENODEV);
		}

		if (info->valid & ACPI_VALID_HID &&
		    !strcmp(info->hardware_id.string, "INT3472")) {
			if (acpi_bus_get_device(dep_devices.handles[i], &dep_adev)) {
				dev_err(dev, "Error getting adev of dep_dev\n");
				return ERR_PTR(-ENODEV);
			}

			/* found adev of dep_dev */
			break;
		}
	}

	if (!dep_adev) {
		dev_err(dev, "adev of dep_dev not found\n");
		return ERR_PTR(-ENODEV);
	}

	/*
	 * HACK: We know that the PMIC is a "discrete" PMIC, an ACPI device
	 * that just serves as a container to list system GPIOs.
	 *
	 * The ACPI device has no fwnode, nor does it have a platform device.
	 * This prevents fetching GPIOs. It however seems to be backed by the
	 * PCI root complex (pci0000:00/0000:00:00.0) as its physical device,
	 * and that device has its fwnode set to \_SB.PCI0.DSC1. Whether this
	 * is correct or not is unknown, let's just get the physical device and
	 * move on for now.
	 */
	dep_phys = list_first_entry_or_null(&dep_adev->physical_node_list,
					    struct acpi_device_physical_node, node);
	if (!dep_phys) {
		dev_info(dev, "Error getting physical node of dep_adev\n");
		return ERR_PTR(-ENODEV);
	}

	dev_info(dev, "Dependent device found: %s\n", dev_name(dep_phys->dev));

	return dep_phys->dev;
}

static int get_acpi_data(struct device *dev)
{
	struct intel_ssdb sensor_data;
	struct intel_cldb pmic_data;
	struct device *dep_dev;
	int len;
	int ret;

	dev_info(dev, "-------------------- %s --------------------\n",
		 dev_name(dev));

	len = read_acpi_block(dev, "SSDB", &sensor_data, sizeof(sensor_data));
	if (len < 0)
		return len;

	dump_ssdb(dev, &sensor_data, len);

	dep_dev = get_dep_dev(dev);
	if (IS_ERR(dep_dev)) {
		ret = PTR_ERR(dep_dev);
		dev_err(dev, "cannot get dep_dev: ret %d\n", ret);
		return ret;
	}

	len = read_acpi_block(dep_dev, "CLDB", &pmic_data, sizeof(pmic_data));
	if (len < 0)
		return len;

	dump_cldb(dep_dev, &pmic_data, len);

	/* FIXME: Calling this sometimes breaks next driver load. */
	// put_device(dep_dev);

	return 0;
}

static int get_acpi_data_probe(struct i2c_client *client)
{
	get_acpi_data(&client->dev);

	return 0;
}

static int get_acpi_data_remove(struct i2c_client *client)
{
	return 0;
}

static const struct acpi_device_id get_acpi_data_acpi_ids[] = {
	{"INT33BE"}, // ov5693
	{"INT3479"}, // ov5670
	{"INT347A"}, // ov8865
	{"INT347E"}, // ov7251
	{"OVTI2680"}, // ov2680
	{"OVTI5648"}, // ov5648
	{"OVTI9734"}, // ov9734
	{},
};
MODULE_DEVICE_TABLE(acpi, get_acpi_data_acpi_ids);

static struct i2c_driver get_acpi_data_i2c_driver = {
	.driver = {
		.acpi_match_table = ACPI_PTR(get_acpi_data_acpi_ids),
		.name  = "get_acpi_data",
	},
	.probe_new  = get_acpi_data_probe,
	.remove = get_acpi_data_remove,
};

module_i2c_driver(get_acpi_data_i2c_driver);

MODULE_LICENSE("GPL v2");
