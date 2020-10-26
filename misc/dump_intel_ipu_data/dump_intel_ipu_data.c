// SPDX-License-Identifier: GPL-2.0

#include <linux/acpi.h>
#include <linux/i2c.h>

#include "dump_intel_ipu_data.h"

#define DRV_NAME	"dump_intel_ipu_data"
#define DRV_VERSION	"0.1"

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
