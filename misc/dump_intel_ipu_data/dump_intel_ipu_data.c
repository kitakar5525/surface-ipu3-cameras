// SPDX-License-Identifier: GPL-2.0

#include <linux/acpi.h>
#include <linux/i2c.h>

#include "dump_intel_ipu_data.h"

#define DRV_NAME	"dump_intel_ipu_data"
#define DRV_VERSION	"1.0"

/**
 * get_acpi_buf - wrapper for acpi_evaluate_object()
 * @adev: ACPI device
 * @path: ACPI object path
 * @out: pointer to point returned buffer from @path
 * @size: buffer size of @out
 *
 * Return negative values for errors.
 * Return 0 for success.
 */
static int get_acpi_buf(struct acpi_device *adev, const char *path,
			void *out, u32 size)
{
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;
	acpi_status status;
	int ret;

	if (!acpi_has_method(handle, (acpi_string)path)) {
		pr_info("ACPI %s: Entry not found\n", path);
		return 0;
	}

	status = acpi_evaluate_object(handle, (acpi_string)path, NULL,
				      &buffer);
	if (ACPI_FAILURE(status)) {
		pr_err("%s: Evaluation failed\n", path);
		return -ENODEV;
	}

	obj = buffer.pointer;
	if (!obj) {
		pr_err("%s: Couldn't locate ACPI buffer\n", path);
		return -ENODEV;
	}

	if (obj->type != ACPI_TYPE_BUFFER) {
		pr_err("%s: Couldn't read ACPI buffer\n", path);
		ret = -ENODEV;
		goto err_free_buf;
	}

	if (obj->buffer.length > size) {
		pr_err("%s: Given buffer is too small\n", path);
		ret = -ENODEV;
		goto err_free_buf;
	}

	memcpy(out, obj->buffer.pointer, obj->buffer.length);
	kfree(buffer.pointer);

	return obj->buffer.length;

err_free_buf:
	kfree(buffer.pointer);
	return ret;
}

/**
 * print_acpi_entry - Print entry for @path
 * @adev: ACPI device
 * @path: ACPI object path to print
 *
 * The type of some entry may be vary depending on devices. For example,
 * _UID is INTEGER or STRING. So, handle all the possible types in this
 * function.
 *
 * Current supported types:
 * - ACPI_TYPE_INTEGER
 * - ACPI_TYPE_STRING
 * - ACPI_TYPE_BUFFER
 */
static int print_acpi_entry(struct acpi_device *adev, const char *path)
{
	struct acpi_handle *handle = adev->handle;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *obj;
	acpi_status status;
	int ret = 0;

	if (!acpi_has_method(handle, (acpi_string)path)) {
		pr_info("ACPI %s: Entry not found\n", path);
		return 0;
	}

	status = acpi_evaluate_object(handle, (acpi_string)path, NULL,
				      &buffer);
	if (ACPI_FAILURE(status)) {
		pr_err("%s: Evaluation failed\n", path);
		return -ENODEV;
	}

	obj = buffer.pointer;
	if (!obj) {
		pr_err("ACPI %s: Couldn't locate ACPI buffer\n", path);
		return -ENODEV;
	}

	switch (obj->type) {
	case ACPI_TYPE_INTEGER:
		pr_info("ACPI %s: %llu\n", path, obj->integer.value);
		goto out_free_buff;
	case ACPI_TYPE_STRING:
		pr_info("ACPI %s: %s\n", path, obj->buffer.pointer);
		goto out_free_buff;
	case ACPI_TYPE_BUFFER:
		pr_info("ACPI %s: Full raw output:\n", path);
		print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
			       obj->buffer.pointer, obj->buffer.length, true);
		goto out_free_buff;
	default:
		pr_err("ACPI %s: Couldn't read ACPI buffer\n", path);
		ret = -ENODEV;
		goto out_free_buff;
	}

out_free_buff:
	kfree(buffer.pointer);
	return ret;
}

static void print_acpi_fullpath(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	char acpi_method_name[255] = { 0 };
	struct acpi_buffer buffer = {sizeof(acpi_method_name), acpi_method_name};

	acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
	pr_info("ACPI path: %s\n", acpi_method_name);
}

static int print_sensor_i2c_dev_name(struct acpi_device *adev)
{
	struct device *i2c_dev;

	i2c_dev = bus_find_device_by_acpi_dev(&i2c_bus_type, adev);
	if (!i2c_dev) {
		pr_warn("%s(): i2c device not found.\n", __func__);
		pr_warn("%s(): Real sensor driver loaded with (old) bridge driver?\n",
			__func__);
		pr_warn("%s(): Otherwise, it might be a problem.\n", __func__);
		pr_warn("%s(): Try adding acpi_enforce_resources=lax to bootloader.\n",
			__func__);
		return -ENODEV;
	}

	pr_info("i2c device name: %s\n", dev_name(i2c_dev));
	put_device(i2c_dev);

	return 0;
}

static int print_pmic_i2c_dev_name(struct acpi_device *adev, int pmic_type)
{
	struct device *i2c_dev;

	i2c_dev = bus_find_device_by_acpi_dev(&i2c_bus_type, adev);
	if (!i2c_dev && !(pmic_type == PMIC_TYPE_DISCRETE)) {
		pr_warn("%s(): non-DISCRETE but i2c device not found\n",
			__func__);
		return -ENODEV;
	} else if (!i2c_dev) {
		pr_info("%s(): (i2c dev not found as expected (DISCRETE))\n",
			__func__);
		return 0;
	}

	if (pmic_type == PMIC_TYPE_DISCRETE) {
		pr_warn("%s(): CLDB indicates DISCRETE but i2c dev %s found\n",
			__func__, dev_name(i2c_dev));
		/* TODO: appropriate return value? */
		return 0;
	}

	pr_info("i2c device name: %s\n", dev_name(i2c_dev));
	put_device(i2c_dev);

	return 0;
}

static int print_dep_acpi_paths(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	struct acpi_handle_list dep_devices;
	const char *path = "_DEP";
	int ret;
	int i;

	if (!acpi_has_method(handle, (acpi_string)path)) {
		pr_info("ACPI %s: Entry not found\n", path);
		return 0;
	}

	ret = acpi_evaluate_reference(handle, (acpi_string)path, NULL,
				      &dep_devices);
	if (ACPI_FAILURE(ret)) {
		pr_err("ACPI %s: Evaluation failed\n", path);
		return -ENODEV;
	}

	for (i = 0; i < dep_devices.count; i++) {
		char acpi_method_name[255] = { 0 };
		struct acpi_buffer buffer = {sizeof(acpi_method_name),
					     acpi_method_name};

		acpi_get_name(dep_devices.handles[i], ACPI_FULL_PATHNAME,
			      &buffer);
		pr_info("ACPI %s (%d of %d): %s\n",
			path, i + 1, dep_devices.count, acpi_method_name);
	}

	if (!dep_devices.count)
		pr_info("ACPI %s: No dependent device found\n", path);

	return 0;
}

static int dump_pld(struct acpi_device *adev)
{
	struct acpi_pld_info *pld;
	struct acpi_handle *handle = adev->handle;
	acpi_status status;
	const char *path = "_PLD";

	pr_info("ACPI %s: ---------- %s() ----------\n", path, __func__);

	if (!acpi_has_method(handle, (acpi_string)path)) {
		pr_info("ACPI %s: Entry not found\n", path);
		return 0;
	}

	status = acpi_get_physical_device_location(handle, &pld);
	if (ACPI_FAILURE(status)) {
		pr_err("ACPI %s: Evaluation failed\n", path);
		return -ENODEV;
	}

	pr_info("revision:            %d\n", pld->revision);
	pr_info("ignore_color:        %d\n", pld->ignore_color);
	pr_info("red:                 %d\n", pld->red);
	pr_info("green:               %d\n", pld->green);
	pr_info("blue:                %d\n", pld->blue);
	pr_info("width:               %d\n", pld->width);
	pr_info("height:              %d\n", pld->height);
	pr_info("user_visible:        %d\n", pld->user_visible);
	pr_info("dock:                %d\n", pld->dock);
	pr_info("lid:                 %d\n", pld->lid);
	pr_info("panel:               %d\n", pld->panel);
	pr_info("vertical_position:   %d\n", pld->vertical_position);
	pr_info("horizontal_position: %d\n", pld->horizontal_position);
	pr_info("shape:               %d\n", pld->shape);
	pr_info("group_orientation:   %d\n", pld->group_orientation);
	pr_info("group_token:         %d\n", pld->group_token);
	pr_info("group_position:      %d\n", pld->group_position);
	pr_info("bay:                 %d\n", pld->bay);
	pr_info("ejectable:           %d\n", pld->ejectable);
	pr_info("ospm_eject_required: %d\n", pld->ospm_eject_required);
	pr_info("cabinet_number:      %d\n", pld->cabinet_number);
	pr_info("card_cage_number:    %d\n", pld->card_cage_number);
	pr_info("reference:           %d\n", pld->reference);
	pr_info("rotation:            %d\n", pld->rotation);
	pr_info("order:               %d\n", pld->order);
	pr_info("reserved:            %d\n", pld->reserved);
	pr_info("vertical_offset:     %d\n", pld->vertical_offset);
	pr_info("horizontal_offset:   %d\n", pld->horizontal_offset);

	pr_info("----- in string -----\n");
	pr_info("PLD_Panel: %s\n", pld_panel_list[pld->panel]);
	pr_info("PLD_VerticalPosition: %s\n",
		pld_vertical_position_list[pld->vertical_position]);
	pr_info("PLD_HorizontalPosition: %s\n",
		pld_horizontal_position_list[pld->horizontal_position]);
	pr_info("PLD_Shape: %s\n", pld_shape_list[pld->shape]);

	return 0;
}

static int dump_crs(struct acpi_device *adev)
{
	const char *path = "_CRS";

	pr_info("ACPI %s: ---------- %s() ----------\n", path, __func__);
	return print_acpi_entry(adev, path);
}

static void dump_ssdb(struct acpi_device *adev, struct intel_ssdb *ssdb,
		      int ssdb_len)
{
	pr_info("ACPI SSDB: ---------- %s() ----------\n", __func__);

	print_acpi_entry(adev, "SSDB");

	pr_info("version:                      %d\n", ssdb->version);
	pr_info("sensor_card_sku:              %d\n", ssdb->sensor_card_sku);
	pr_info("csi2_data_stream_interface:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       ssdb->csi2_data_stream_interface,
		       sizeof(ssdb->csi2_data_stream_interface), true);
	pr_info("bdf_value:                    %d\n", ssdb->bdf_value);
	pr_info("dphy_link_en_fuses:           %d\n", ssdb->dphy_link_en_fuses);
	pr_info("lanes_clock_division:         %d\n", ssdb->lanes_clock_division);
	pr_info("link_used:                    %d\n", ssdb->link_used);
	pr_info("lanes_used:                   %d\n", ssdb->lanes_used);
	pr_info("csi_rx_dly_cnt_termen_clane:  %d\n", ssdb->csi_rx_dly_cnt_termen_clane);
	pr_info("csi_rx_dly_cnt_settle_clane:  %d\n", ssdb->csi_rx_dly_cnt_settle_clane);
	pr_info("csi_rx_dly_cnt_termen_dlane0: %d\n", ssdb->csi_rx_dly_cnt_termen_dlane0);
	pr_info("csi_rx_dly_cnt_settle_dlane0: %d\n", ssdb->csi_rx_dly_cnt_settle_dlane0);
	pr_info("csi_rx_dly_cnt_termen_dlane1: %d\n", ssdb->csi_rx_dly_cnt_termen_dlane1);
	pr_info("csi_rx_dly_cnt_settle_dlane1: %d\n", ssdb->csi_rx_dly_cnt_settle_dlane1);
	pr_info("csi_rx_dly_cnt_termen_dlane2: %d\n", ssdb->csi_rx_dly_cnt_termen_dlane2);
	pr_info("csi_rx_dly_cnt_settle_dlane2: %d\n", ssdb->csi_rx_dly_cnt_settle_dlane2);
	pr_info("csi_rx_dly_cnt_termen_dlane3: %d\n", ssdb->csi_rx_dly_cnt_termen_dlane3);
	pr_info("csi_rx_dly_cnt_settle_dlane3: %d\n", ssdb->csi_rx_dly_cnt_settle_dlane3);
	pr_info("max_lane_speed:               %d\n", ssdb->max_lane_speed);
	pr_info("sensor_cal_file_idx:          %d\n", ssdb->sensor_cal_file_idx);
	pr_info("sensor_cal_file_idx_mbz:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       ssdb->sensor_cal_file_idx_mbz,
		       sizeof(ssdb->sensor_cal_file_idx_mbz), true);
	pr_info("rom_type:                     %d\n", ssdb->rom_type);
	pr_info("vcm_type:                     %d\n", ssdb->vcm_type);
	pr_info("platform:                     %d\n", ssdb->platform);
	pr_info("platform_sub:                 %d\n", ssdb->platform_sub);
	pr_info("flash_support:                %d\n", ssdb->flash_support);
	pr_info("privacy_led:                  %d\n", ssdb->privacy_led);
	pr_info("degree:                       %d\n", ssdb->degree);
	pr_info("mipi_define:                  %d\n", ssdb->mipi_define);
	pr_info("mclk_speed:                   %d\n", ssdb->mclk_speed);
	pr_info("control_logic_id:             %d\n", ssdb->control_logic_id);
	pr_info("mipi_data_format:             %d\n", ssdb->mipi_data_format);
	pr_info("silicon_version:              %d\n", ssdb->silicon_version);
	pr_info("customer_id:                  %d\n", ssdb->customer_id);
	pr_info("mclk_port:                    %d\n", ssdb->mclk_port);
	pr_info("reserved:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       ssdb->reserved, sizeof(ssdb->reserved), true);

	pr_info("----- excerpt -----\n");
	pr_info("link_used:     %d\n", ssdb->link_used);
	pr_info("lanes_used:    %d\n", ssdb->lanes_used);
	pr_info("vcm_type:      %d\n", ssdb->vcm_type);
	pr_info("flash_support: %d\n", ssdb->flash_support);
	pr_info("degree:        %d\n", ssdb->degree);
	pr_info("mclk_speed:    %d\n", ssdb->mclk_speed);
	pr_info("mclk_port:     %d\n", ssdb->mclk_port);
}

static void dump_cldb(struct acpi_device *adev, struct intel_cldb *cldb,
		      int cldb_len)
{
	pr_info("ACPI CLDB: ---------- %s() ----------\n", __func__);

	print_acpi_entry(adev, "CLDB");

	pr_info("version:            %d\n", cldb->version);
	pr_info("control_logic_type: %d\n", cldb->control_logic_type);
	pr_info("control_logic_id:   %d\n", cldb->control_logic_id);
	pr_info("sensor_card_sku:    %d\n", cldb->sensor_card_sku);
	pr_info("reserved:\n");
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       cldb->reserved, sizeof(cldb->reserved), true);
}

static void print_pmic_type(struct acpi_device *adev, struct intel_cldb *data)
{
	switch (data->control_logic_type) {
	case PMIC_TYPE_DISCRETE:
		pr_info("ACPI CLDB: PMIC type is %s\n",
			control_logic_type_list[data->control_logic_type]);
		break;
	case PMIC_TYPE_UNKNOWN:
	case PMIC_TYPE_TPS68470:
	case PMIC_TYPE_UP6641:
		pr_warn("ACPI CLDB: PMIC type is %s\n",
			control_logic_type_list[data->control_logic_type]);
		break;
	default:
		pr_warn("PMIC type %d is not known type\n",
			data->control_logic_type);
	}
}

/**
 * get_dsm_data_string - wrapper for acpi_evaluate_dsm()
 * @adev: ACPI device
 * @guid: GUID of requested functions, should be 16 bytes
 * @dsm_rev: revision number of requested function
 * @dsm_func: requested function number
 * @out: pointer to string to point returned string from _DSM
 * @size: buffer size of @out
 *
 * Return negative values for errors.
 * Return 0 for success.
 */
static int get_dsm_data_string(struct acpi_device *adev, const guid_t *guid,
			       int dsm_rev, int dsm_func,
			       char *out, unsigned int size)
{
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;
	int ret = 0;

	obj = acpi_evaluate_dsm_typed(handle, guid, dsm_rev, dsm_func,
				      NULL, ACPI_TYPE_STRING);
	if (!obj) {
		pr_debug("%s(): _DSM execution failed. GUID not exist?\n",
			 __func__);
		return -ENODEV;
	}

	if (!obj->string.pointer) {
		pr_err("%s(): Couldn't locate ACPI string pointer\n",
		       __func__);
		ret = -ENODEV;
		goto free_obj;
	}

	/* The length string.length field does not include the terminating
	 * NULL, add 1 for it.
	 */
	if (size < obj->string.length + 1)
		pr_warn("%s(): Given buffer is too small, truncating\n",
			__func__);
	strlcpy(out, obj->string.pointer,
		min(obj->string.length + 1, size));

free_obj:
	ACPI_FREE(obj);
	return ret;
}

/**
 * get_dsm_data_integer - wrapper for acpi_evaluate_dsm()
 * @adev: ACPI device
 * @guid: GUID of requested functions, should be 16 bytes
 * @rev: revision number of requested function
 * @func: requested function number
 * @out: pointer to integer to point returned integer value from _DSM
 *
 * Return negative values for errors.
 * Return 0 for success.
 */
static int get_dsm_data_integer(struct acpi_device *adev, const guid_t *guid,
				int dsm_rev, int dsm_func, u64 *out)
{
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;

	obj = acpi_evaluate_dsm_typed(handle, guid, dsm_rev, dsm_func,
				      NULL, ACPI_TYPE_INTEGER);
	if (!obj) {
		pr_debug("%s(): _DSM execution failed. GUID not exist?\n",
			 __func__);
		return -ENODEV;
	}

	/* TODO: Even if the GUID doesn't exist, it seems that
	 * acpi_evaluate_dsm_typed() doesn't fail and obj->integer.value
	 * will be 0 anyway. Until we find a better way to tell if the
	 * GUID really doesn't exist, ignore this case.
	 */
	*out = obj->integer.value;

	ACPI_FREE(obj);
	return 0;
}

static int __dump_subsys_id_dsm(struct acpi_device *adev)
{
	char id[DSM_STR_BUF_SIZE];
	int ret;

	ret = get_dsm_data_string(adev, &subsys_id_dsm_guid,
				  SUBSYS_ID_DSM_REV,
				  SUBSYS_ID_DSM_RETURN_ID_FUNC,
				  id, DSM_STR_BUF_SIZE);
	if (ret) {
		pr_info("%s(): Couldn't get Subsystem ID. GUID not exist?\n",
			__func__);
		return 0;
	}

	pr_info("%s(): Subsystem ID: %s\n", __func__, id);

	return 0;
}

static int __dump_i2c_dev_dsm(struct acpi_device *adev)
{
	u64 dev_amount;
	int ret;
	int i;

	ret = get_dsm_data_integer(adev, &i2c_dev_dsm_guid,
				   I2C_DEV_DSM_REV,
				   I2C_DEV_DSM_DEV_AMOUNT_FUNC,
				   &dev_amount);
	if (ret) {
		pr_info("%s(): Couldn't get i2c dev amount. GUID not exist?\n",
			__func__);
		return 0;
	}

	pr_info("%s(): i2c device amount: %llu\n", __func__, dev_amount);

	/* dump _DSM data for each device */
	for (i = 1; i <= dev_amount; i++) {
		u64 dev_dsm_data;
		u16 i2c_bus;
		u16 i2c_second_byte;
		u16 i2c_addr;
		u16 i2c_dev_type;

		ret = get_dsm_data_integer(adev, &i2c_dev_dsm_guid,
					   I2C_DEV_DSM_REV,
					   I2C_DEV_DSM_DEV_AMOUNT_FUNC + i,
					   &dev_dsm_data);
		if (ret) {
			pr_err("%s(): Couldn't get i2c dev %d data\n",
			       __func__, i);
			return -EIO;
		}

		/* TODO: The original ipu4-acpi subtracts 1 from i2c_bus. Why?
		 * Not substracting here.
		 * https://github.com/intel/linux-intel-lts/blob/8e69644ab5b84be1400874ac1dbcb4389aa2412c/drivers/media/platform/intel/ipu4-acpi.c#L337
		 */
		i2c_bus = (dev_dsm_data >> 24) & 0xff;
		i2c_second_byte = (dev_dsm_data >> 16) & 0xff;
		i2c_addr = (dev_dsm_data >> 8) & 0xff;
		/* The last byte is i2c device type
		 * https://github.com/coreboot/coreboot/blob/b38d6bbe1c74309f078915c2d467475bb4144943/src/drivers/intel/mipi_camera/chip.h#L27
		 * https://github.com/coreboot/coreboot/blob/b38d6bbe1c74309f078915c2d467475bb4144943/src/drivers/intel/mipi_camera/camera.c#L196
		 */
		i2c_dev_type = dev_dsm_data & 0xff;

		pr_info("%s(): i2c device _DSM data (%d of %llu): 0x%08llx, bus: 0x%02x, second_byte: 0x%02x, addr: 0x%02x, dev_type: 0x%02x\n",
			__func__, i, dev_amount, dev_dsm_data,
			i2c_bus, i2c_second_byte, i2c_addr, i2c_dev_type);
	}

	return 0;
}

static int __dump_discrete_pmic_dsm(struct acpi_device *adev)
{
	u64 gpio_pin_amount;
	int ret;
	int i;

	ret = get_dsm_data_integer(adev, &pmic_dsm_guid,
				   DISCRETE_PMIC_DSM_REV,
				   DISCRETE_PMIC_DSM_GPIO_AMOUNT_FUNC,
				   &gpio_pin_amount);
	if (ret) {
		pr_info("%s(): Couldn't get GPIO pin amount func. GUID not exist?\n",
			__func__);
		return 0;
	}

	pr_info("%s(): GPIO pin amount: %llu\n", __func__, gpio_pin_amount);

	/* dump _DSM data for each GPIO pin */
	for (i = 1; i <= gpio_pin_amount; i++) {
		u64 gpio_dsm_data;
		u16 gpio_first_byte;
		u16 gpio_second_byte;
		u16 gpio_pin_num;
		u16 gpio_last_byte;

		ret = get_dsm_data_integer(adev, &pmic_dsm_guid,
					   DISCRETE_PMIC_DSM_REV,
					   DISCRETE_PMIC_DSM_GPIO_AMOUNT_FUNC + i,
					   &gpio_dsm_data);
		if (ret) {
			pr_err("%s(): Couldn't get GPIO pin %d data\n",
			       __func__, i);
			return -EIO;
		}

		gpio_first_byte = (gpio_dsm_data >> 24) & 0xff;
		gpio_second_byte = (gpio_dsm_data >> 16) & 0xff;
		gpio_pin_num = (gpio_dsm_data >> 8) & 0xff;
		gpio_last_byte = gpio_dsm_data & 0xff;

		pr_info("%s(): GPIO pin _DSM data (%d of %llu): 0x%08llx, first_byte: 0x%02x, second_byte: 0x%02x, pin_num: 0x%02x, last_byte: 0x%02x\n",
			__func__, i, gpio_pin_amount, gpio_dsm_data,
			gpio_first_byte, gpio_second_byte,
			gpio_pin_num, gpio_last_byte);
	}

	return 0;
}

static int __dump_dsmb_dsm(struct acpi_device *adev)
{
	struct acpi_handle *handle = adev->handle;
	union acpi_object *obj;

	/* TODO: abstract this like the other get_dsm_data_{string,integer}?
	 * but we don't know how much buffer size we should prepare.
	 */
	obj = acpi_evaluate_dsm_typed(handle, &dsmb_dsm_guid,
				      DSMB_DSM_REV,
				      DSMB_DSM_RETURN_BUF_FUNC,
				      NULL, ACPI_TYPE_BUFFER);
	if (!obj) {
		pr_info("%s(): _DSM failed for getting DSMB buffer func. GUID not exist?\n",
			__func__);
		return 0;
	}

	pr_info("%s(): Full raw output of DSMB:\n", __func__);
	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1,
		       obj->buffer.pointer, obj->buffer.length, true);

	ACPI_FREE(obj);

	return 0;
}

static void dump_dsm(struct acpi_device *adev)
{
	/* Some GUIDs for _DSM may not exist. So, not checking return
	 * values.
	 */
	__dump_subsys_id_dsm(adev);
	__dump_i2c_dev_dsm(adev);
	__dump_discrete_pmic_dsm(adev);
	__dump_dsmb_dsm(adev);
}

static int get_acpi_sensor_data(struct acpi_device *adev)
{
	struct intel_ssdb sensor_data;
	int ssdb_len;

	pr_info("%s(): ==================== %s (Sensor) ====================\n",
		__func__, dev_name(&adev->dev));

	ssdb_len = get_acpi_buf(adev, "SSDB", &sensor_data, sizeof(sensor_data));
	if (!ssdb_len) {
		pr_info("%s(): Reading SSDB failed\n", __func__);
		return ssdb_len;
	}

	print_acpi_entry(adev, "_ADR");
	print_acpi_entry(adev, "_HID");
	print_acpi_entry(adev, "_CID");
	print_acpi_entry(adev, "_DDN");
	print_acpi_entry(adev, "_SUB");
	print_acpi_entry(adev, "_UID");

	print_acpi_fullpath(adev);
	pr_info("ACPI device name: %s\n", dev_name(&adev->dev));
	print_sensor_i2c_dev_name(adev);
	print_dep_acpi_paths(adev);

	dump_pld(adev);
	dump_crs(adev);
	dump_ssdb(adev, &sensor_data, ssdb_len);
	dump_dsm(adev);

	return 0;
}

static int get_acpi_pmic_data(struct acpi_device *adev)
{
	struct intel_cldb pmic_data;
	int cldb_len;

	pr_info("%s(): ==================== %s (PMIC) ====================\n",
		__func__, dev_name(&adev->dev));

	cldb_len = get_acpi_buf(adev, "CLDB", &pmic_data, sizeof(pmic_data));
	if (!cldb_len) {
		pr_info("%s(): Reading CLDB failed\n", __func__);
		return cldb_len;
	}

	print_acpi_entry(adev, "_ADR");
	print_acpi_entry(adev, "_HID");
	print_acpi_entry(adev, "_CID");
	print_acpi_entry(adev, "_DDN");
	print_acpi_entry(adev, "_SUB");
	print_acpi_entry(adev, "_UID");

	print_acpi_fullpath(adev);
	pr_info("ACPI device name: %s\n", dev_name(&adev->dev));
	print_pmic_i2c_dev_name(adev, pmic_data.control_logic_type);
	print_dep_acpi_paths(adev);

	dump_pld(adev);
	dump_crs(adev);
	dump_cldb(adev, &pmic_data, cldb_len);
	print_pmic_type(adev, &pmic_data);
	dump_dsm(adev);

	return 0;
}

/* to use acpi_driver.drv.bus (acpi_bus_type) */
static struct acpi_driver dump_intel_ipu_data_driver = {
	.name = DRV_NAME,
	.class = DRV_NAME,
};

static bool is_supported_sensor(struct acpi_device *adev)
{
	/* check if SSDB is present */
	if (!acpi_has_method(adev->handle, "SSDB")) {
		dev_dbg(&adev->dev, "SSDB not found\n");
		return false;
	}

	return true;
}

static bool is_supported_pmic(struct acpi_device *adev)
{
	/* check if CLDB is present */
	if (!acpi_has_method(adev->handle, "CLDB")) {
		dev_dbg(&adev->dev, "CLDB not found\n");
		return false;
	}

	return true;
}

static int acpi_dev_match_cb(struct device *dev, void *data)
{
	struct acpi_device *adev = to_acpi_device(dev);
	struct device_count *dev_cnt = data;
	int ret;

	/* check if the device really exists */
	ret = acpi_bus_get_status(adev);
	if (ret) {
		dev_dbg(dev, "acpi_bus_get_status() failed, ret = %d\n", ret);
		return 0;
	}
	if (!adev->status.present) {
		dev_dbg(dev, "Device not present\n");
		return 0;
	}

	if (is_supported_sensor(adev)) {
		dev_cnt->sensor++;
		get_acpi_sensor_data(adev);
		pr_info("\n");
	}

	if (is_supported_pmic(adev)) {
		dev_cnt->pmic++;
		get_acpi_pmic_data(adev);
		pr_info("\n");
	}

	return 0;
}

static int __init dump_intel_ipu_data_init(void)
{
	struct device_count dev_cnt;
	int ret;

	pr_info(DRV_NAME ": Version %s init\n", DRV_VERSION);

	ret = acpi_bus_register_driver(&dump_intel_ipu_data_driver);
	if (ret) {
		pr_err(DRV_NAME ": Failed registering acpi driver: %d\n", ret);
		return ret;
	}

	/* iterate over all ACPI devices */
	bus_for_each_dev(dump_intel_ipu_data_driver.drv.bus, NULL, &dev_cnt,
			 acpi_dev_match_cb);

	pr_info(DRV_NAME ": Found %d supported sensor(s)\n", dev_cnt.sensor);
	pr_info(DRV_NAME ": Found %d supported PMIC(s)\n", dev_cnt.pmic);

	return 0;
}

static void __exit dump_intel_ipu_data_exit(void)
{
	acpi_bus_unregister_driver(&dump_intel_ipu_data_driver);
}

module_init(dump_intel_ipu_data_init);
module_exit(dump_intel_ipu_data_exit);

MODULE_AUTHOR("Tsuchiya Yuto <kitakar@gmail.com>");
MODULE_DESCRIPTION("Dump camera data for Intel IPU systems");
MODULE_VERSION(DRV_VERSION);
MODULE_LICENSE("GPL");
