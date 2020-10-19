// SPDX-License-Identifier: GPL-2.0

#include <linux/acpi.h>
#include <linux/i2c.h>

#include "get_acpi_data.h"

/* This list will be used for printing sensor names. This list is needed
 * because not all systems contain the sensor names data in its DSDT...
 */
static const struct ipu3_sensor ipu3_sensors[] = {
	// {"ACPI_HID", "sensor_name"},
	{"INT33BE", "ov5693"},
	{"INT3479", "ov5670"},
	{"INT347A", "ov8865"},
	{"INT347E", "ov7251"},
	{"OVTI2680", "ov2680"},
	{"OVTI5648", "ov5648"},
	{"OVTI9734", "ov9734"},
	{},
};

static void dump_ssdb(struct acpi_device *adev, struct intel_ssdb *data, int data_len)
{
	dev_info(&adev->dev, "========== %s() ==========\n", __func__);

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

static void dump_cldb(struct acpi_device *adev, struct intel_cldb *data, int data_len)
{
	dev_info(&adev->dev, "========== %s() ==========\n", __func__);

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

static int dump_crs(struct acpi_device *adev)
{
	union acpi_object *obj;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	struct acpi_handle *dev_handle = adev->handle;
	int status;

	dev_info(&adev->dev, "========== %s() ==========\n", __func__);

	status = acpi_evaluate_object(dev_handle, "_CRS", NULL, &buffer);
	if (!ACPI_SUCCESS(status))
		return -ENODEV;

	obj = (union acpi_object *)buffer.pointer;
	if (!obj || obj->type != ACPI_TYPE_BUFFER) {
		dev_err(&adev->dev, "Could't read acpi buffer\n");
		status = -ENODEV;
		goto err;
	}

	pr_info("full raw output of _CRS:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       obj->buffer.pointer, obj->buffer.length, true);
	kfree(buffer.pointer);

	return 0;
err:
	kfree(buffer.pointer);
	return status;
}

static int read_acpi_block(struct acpi_device *adev, char *id, void *data, u32 size)
{
	union acpi_object *obj;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	struct acpi_handle *dev_handle = adev->handle;
	int status;
	u32 buffer_length;

	status = acpi_evaluate_object(dev_handle, id, NULL, &buffer);
	if (!ACPI_SUCCESS(status)) {
		dev_err(&adev->dev, "acpi_evaluate_object() failed\n");
		return -ENODEV;
	}

	obj = (union acpi_object *)buffer.pointer;
	if (!obj || obj->type != ACPI_TYPE_BUFFER) {
		dev_err(&adev->dev, "Could't read acpi buffer\n");
		status = -ENODEV;
		goto err;
	}

	if (obj->buffer.length > size) {
		dev_err(&adev->dev, "Given buffer is too small\n");
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

static void print_acpi_path(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	char acpi_method_name[255] = { 0 };
	struct acpi_buffer buffer = {sizeof(acpi_method_name), acpi_method_name};

	acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
	pr_info("ACPI path: %s\n", acpi_method_name);
}

static int print_dep_acpi_paths(struct acpi_device *adev)
{
	struct acpi_handle *dev_handle = adev->handle;
	struct acpi_handle_list dep_devices;
	int ret;
	int i;

	if (!acpi_has_method(dev_handle, "_DEP")) {
		/* There may be no _DEP defined (especially for PMIC).
		 * So, treat this as not an error.
		 */
		pr_info("%s(): No _DEP entry found\n", __func__);
		return 0;
	}

	ret = acpi_evaluate_reference(dev_handle, "_DEP", NULL, &dep_devices);
	if (ACPI_FAILURE(ret)) {
		dev_err(&adev->dev, "Failed to evaluate _DEP.\n");
		return -ENODEV;
	}

	for (i = 0; i < dep_devices.count; i++) {
		char acpi_method_name[255] = { 0 };
		struct acpi_buffer buffer = {sizeof(acpi_method_name),
					     acpi_method_name};

		acpi_get_name(dep_devices.handles[i], ACPI_FULL_PATHNAME,
			      &buffer);
		pr_info("_DEP ACPI path (%d of %d): %s\n",
			i + 1, dep_devices.count,
			acpi_method_name);
	}

	if (!dep_devices.count)
		pr_info("%s(): No dependent device found\n", __func__);

	return 0;
}

static int print_i2c_dev_name(struct acpi_device *adev)
{
	struct device *i2c_dev;

	i2c_dev = bus_find_device_by_acpi_dev(&i2c_bus_type, adev);
	if (!i2c_dev) {
		pr_info("%s(): i2c device not found. Possible reasons:\n",
			__func__);
		pr_info("%s(): 1) It's a PMIC and the PMIC type is DISCRETE.\n",
			__func__);
		pr_info("%s(): 2) It's a sensor device and the real sensor driver is loaded with the bridge driver.\n",
			__func__);
		pr_info("%s(): Otherwise, it might be a problem. Try adding acpi_enforce_resources=lax to your bootloader.\n",
			__func__);
	} else {
		pr_info("i2c device name: %s\n", dev_name(i2c_dev));
	}

	return 0;
}

static void print_sensor_name(struct acpi_device *adev)
{
	int idx;

	for (idx = 0; idx < ARRAY_SIZE(ipu3_sensors); idx++) {
		if (!strcmp(ipu3_sensors[idx].acpi_hid, acpi_device_hid(adev)))
			break;
	}

	if (idx >= ARRAY_SIZE(ipu3_sensors)) {
		pr_info("%s(): Couldn't find sensor name\n", __func__);
		return;
	}

	pr_info("Sensor name: %s\n", ipu3_sensors[idx].sensor_name);
}

static void print_pmic_type(struct acpi_device *adev, struct intel_cldb *data)
{
	switch (data->control_logic_type) {
	case 0:
		pr_warn("%s(): PMIC type is \"0: UNKNOWN\"\n", __func__);
		return;
	case 1:
		pr_info("%s(): PMIC type is \"1: DISCRETE(CRD-D)\"\n",
			__func__);
		return;
	case 2:
		pr_warn("%s(): PMIC type is \"2: PMIC TPS68470\"\n", __func__);
		return;
	case 3:
		pr_warn("%s(): PMIC type is \"3: PMIC uP6641\"\n", __func__);
		return;
	}

	pr_err("%s(): PMIC type %d is not known type\n",
	       __func__, data->control_logic_type);
}

static int __dump_subsys_id_dsm(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;

	obj = acpi_evaluate_dsm_typed(handle, &subsys_id_dsm_guid,
				      SUBSYS_ID_DSM_REV,
				      SUBSYS_ID_DSM_RETURN_ID_FUNC,
				      NULL, ACPI_TYPE_STRING);
	if (!obj) {
		pr_info("%s(): _DSM failed for getting subsystem ID func. Maybe it doesn't exist.\n",
			__func__);
		return 0;
	}

	pr_info("%s(): Subsystem ID: %s\n", __func__, obj->string.pointer);
	ACPI_FREE(obj);

	return 0;
}

static int __dump_i2c_dev_dsm(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;
	int device_amount;
	int device_dsm_data;
	int i;

	obj = acpi_evaluate_dsm_typed(handle, &i2c_dev_dsm_guid,
				      I2C_DEV_DSM_REV,
				      I2C_DEV_DSM_DEV_AMOUNT_FUNC,
				      NULL, ACPI_TYPE_INTEGER);
	if (!obj) {
		pr_info("%s(): _DSM failed for getting i2c dev amount func. Maybe it doesn't exist.\n",
			__func__);
		return 0;
	}
	device_amount = obj->integer.value;
	ACPI_FREE(obj);

	pr_info("%s(): i2c device amount: %d\n", __func__, device_amount);

	/* dump _DSM data for each device */
	for (i = 1; i <= device_amount; i++) {
		obj = acpi_evaluate_dsm_typed(handle, &i2c_dev_dsm_guid,
					      I2C_DEV_DSM_REV,
					      I2C_DEV_DSM_DEV_AMOUNT_FUNC + i,
					      NULL, ACPI_TYPE_INTEGER);
		if (!obj) {
			dev_err(&adev->dev,
				"_DSM failed for getting i2c dev %d data func\n", i);
			return -EIO;
		}
		device_dsm_data = obj->integer.value;
		ACPI_FREE(obj);

		pr_info("%s(): i2c device _DSM data (%d of %d): 0x%08x\n",
			__func__, i, device_amount, device_dsm_data);
	}

	return 0;
}

static int __dump_discrete_pmic_dsm(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;
	int gpio_pin_amount;
	int gpio_dsm_data;
	int i;

	obj = acpi_evaluate_dsm_typed(handle, &pmic_dsm_guid,
				      DISCRETE_PMIC_DSM_REV,
				      DISCRETE_PMIC_DSM_GPIO_AMOUNT_FUNC,
				      NULL, ACPI_TYPE_INTEGER);
	if (!obj) {
		pr_info("%s(): _DSM failed for getting GPIO pin amount func. Maybe it doesn't exist.\n",
			__func__);
		return 0;
	}
	gpio_pin_amount = obj->integer.value;
	ACPI_FREE(obj);

	pr_info("%s(): GPIO pin amount: %d\n", __func__, gpio_pin_amount);

	/* dump _DSM data for each GPIO pin */
	for (i = 1; i <= gpio_pin_amount; i++) {
		obj = acpi_evaluate_dsm_typed(handle, &pmic_dsm_guid,
					      DISCRETE_PMIC_DSM_REV,
					      DISCRETE_PMIC_DSM_GPIO_AMOUNT_FUNC + i,
					      NULL, ACPI_TYPE_INTEGER);
		if (!obj) {
			dev_err(&adev->dev,
				"_DSM failed for getting GPIO pin %d data func\n", i);
			return -EIO;
		}
		gpio_dsm_data = obj->integer.value;
		ACPI_FREE(obj);

		pr_info("%s(): GPIO pin _DSM data (%d of %d): 0x%08x\n",
			__func__, i, gpio_pin_amount, gpio_dsm_data);
	}

	return 0;
}

static int __dump_dsmb_dsm(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;

	obj = acpi_evaluate_dsm_typed(handle, &dsmb_dsm_guid,
				      DSMB_DSM_REV,
				      DSMB_DSM_RETURN_BUF_FUNC,
				      NULL, ACPI_TYPE_BUFFER);
	if (!obj) {
		pr_info("%s(): _DSM failed for getting DSMB buffer func. Maybe it doesn't exist.\n",
			__func__);
		return 0;
	}

	pr_info("%s(): full raw output of DSMB:\n", __func__);
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       obj->buffer.pointer, obj->buffer.length, true);

	ACPI_FREE(obj);

	return 0;
}

static void dump_dsm(struct acpi_device *adev)
{
	/* Some _DSM may not exist. So, not checking return values. */
	__dump_subsys_id_dsm(adev);
	__dump_i2c_dev_dsm(adev);
	__dump_discrete_pmic_dsm(adev);
	__dump_dsmb_dsm(adev);
}

static int get_acpi_sensor_data(struct acpi_device *adev)
{
	struct intel_ssdb sensor_data;
	int len;

	len = read_acpi_block(adev, "SSDB", &sensor_data, sizeof(sensor_data));
	if (len < 0)
		return len;

	dev_info(&adev->dev, "-------------------- %s --------------------\n",
		 dev_name(&adev->dev));

	print_sensor_name(adev);
	print_acpi_path(adev);
	pr_info("ACPI device name: %s\n", dev_name(&adev->dev));
	print_i2c_dev_name(adev);
	print_dep_acpi_paths(adev);
	dump_crs(adev);
	dump_ssdb(adev, &sensor_data, len);
	dump_dsm(adev);
	pr_info("\n");

	return 0;
}

static int get_acpi_pmic_data(struct acpi_device *adev)
{
	struct intel_cldb pmic_data;
	int len;

	len = read_acpi_block(adev, "CLDB", &pmic_data, sizeof(pmic_data));
	if (len < 0)
		return len;

	dev_info(&adev->dev, "-------------------- %s --------------------\n",
		 dev_name(&adev->dev));

	print_acpi_path(adev);
	pr_info("ACPI device name: %s\n",
		dev_name(&adev->dev));
	print_i2c_dev_name(adev);
	print_dep_acpi_paths(adev);
	dump_crs(adev);
	dump_cldb(adev, &pmic_data, len);
	print_pmic_type(adev, &pmic_data);
	dump_dsm(adev);
	pr_info("\n");

	return 0;
}

/* to use acpi_driver.drv.bus (acpi_bus_type) */
static struct acpi_driver get_acpi_data_driver = {
	.name = DRV_NAME,
	.class = DRV_NAME,
};

static int acpi_dev_match_cb(struct device *dev, void *data)
{
	struct acpi_device *adev = to_acpi_device(dev);

	get_acpi_sensor_data(adev);
	get_acpi_pmic_data(adev);

	return 0;
}

static int __init get_acpi_data_init(void)
{
	int ret;

	ret = acpi_bus_register_driver(&get_acpi_data_driver);
	if (ret) {
		pr_err(DRV_NAME": registering acpi driver failed\n");
		return ret;
	}

	/* iterate over all ACPI devices  */
	bus_for_each_dev(get_acpi_data_driver.drv.bus, NULL, NULL,
			 acpi_dev_match_cb);

	return 0;
}

static void __exit get_acpi_data_exit(void)
{
	acpi_bus_unregister_driver(&get_acpi_data_driver);
}

module_init(get_acpi_data_init);
module_exit(get_acpi_data_exit);

MODULE_LICENSE("GPL v2");
