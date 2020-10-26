// SPDX-License-Identifier: GPL-2.0

#include <linux/acpi.h>

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
