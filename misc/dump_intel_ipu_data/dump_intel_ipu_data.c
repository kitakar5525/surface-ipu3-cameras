// SPDX-License-Identifier: GPL-2.0

#include <linux/acpi.h>

#define DRV_NAME	"dump_intel_ipu_data"
#define DRV_VERSION	"0.1"

static int get_acpi_sensor_data(struct acpi_device *adev)
{
	pr_info("%s(): ==================== %s (Sensor) ====================\n",
		__func__, dev_name(&adev->dev));

	return 0;
}

static int get_acpi_pmic_data(struct acpi_device *adev)
{
	pr_info("%s(): ==================== %s (PMIC) ====================\n",
		__func__, dev_name(&adev->dev));

	return 0;
}

/* to use acpi_driver.drv.bus (acpi_bus_type) */
static struct acpi_driver dump_intel_ipu_data_driver = {
	.name = DRV_NAME,
	.class = DRV_NAME,
};

static int acpi_dev_match_cb(struct device *dev, void *data)
{
	struct acpi_device *adev = to_acpi_device(dev);
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

	get_acpi_sensor_data(adev);
	pr_info("\n");
	get_acpi_pmic_data(adev);
	pr_info("\n");

	return 0;
}

static int __init dump_intel_ipu_data_init(void)
{
	int ret;

	pr_info(DRV_NAME ": Version %s init\n", DRV_VERSION);

	ret = acpi_bus_register_driver(&dump_intel_ipu_data_driver);
	if (ret) {
		pr_err(DRV_NAME ": Failed registering acpi driver: %d\n", ret);
		return ret;
	}

	/* iterate over all ACPI devices */
	bus_for_each_dev(dump_intel_ipu_data_driver.drv.bus, NULL, NULL,
			 acpi_dev_match_cb);

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
