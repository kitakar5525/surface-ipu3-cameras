#include <linux/acpi.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>

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

static int get_acpi_cldb_pmic_data(struct device *dev,
				     struct intel_cldb *cldb)
{
	int ret;
	
	ret = read_acpi_block(dev, "CLDB", &cldb, sizeof(cldb));
	if (ret < 0)
		return ret;

	pr_info("version: %d\n", cldb->version);
	pr_info("control_logic_type: %d\n", cldb->control_logic_type);
	pr_info("control_logic_id: %d\n", cldb->control_logic_id);
	pr_info("sensor_card_sku: %d\n", cldb->sensor_card_sku);
	pr_info("\n");

	return 0;
}

static int tps68470_dump_cldb_probe(void)
{
	struct acpi_device *int3472_adev;
	struct device *int3472_dev;
	/* SGo1/SGo2 has i2c-INT3472:05 */
	char *uid = "5";
	struct intel_cldb cldb;

	int3472_adev = acpi_dev_get_first_match_dev("INT3472", uid, -1);
	if (!int3472_adev) {
		pr_err("Error getting int3472 acpi_device\n");
		return -ENODEV;
	}

	/* Note: if platform_bus_type doesn't work, pci_bus_type might
	 * work
	 */
	int3472_dev = bus_find_device_by_acpi_dev(&platform_bus_type, int3472_adev);
	acpi_dev_put(int3472_adev);
	if (!int3472_dev) {
		pr_err("Error getting int3472 device\n");
		return -ENODEV;
	}

	get_acpi_cldb_pmic_data(int3472_dev, &cldb);

	put_device(int3472_dev);

	return 0;
}

static int tps68470_dump_cldb_remove(void)
{
	return 0;
}

static int __init kernel_module_init(void)
{
	pr_info("module_init()\n");

	tps68470_dump_cldb_probe();

	return 0;
}

static void __exit kernel_module_exit(void)
{
	pr_info("module_exit()\n");

	tps68470_dump_cldb_remove();
}

module_init(kernel_module_init);
module_exit(kernel_module_exit);

MODULE_LICENSE("GPL v2");
