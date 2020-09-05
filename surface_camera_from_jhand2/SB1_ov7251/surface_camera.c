#include <linux/acpi.h>
#include <linux/device.h>
#include <linux/fwnode.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/pci.h>
#include <media/v4l2-subdev.h>

#define SWNODE_OV7251			0
#define SWNODE_OV7251_PORT0		1
#define SWNODE_OV7251_ENDPOINT0	2

#define SWNODE_CIO2				3
#define SWNODE_CIO2_PORT0		4
#define SWNODE_CIO2_ENDPOINT0	5

#define SURFACE_OV7251_HID	"INT347E"
#define SURFACE_CIO2_HID	"INT343E"

#define CIO2_PCI_ID			0x9d32

struct surface_camera {
	struct pci_dev *cio2;
	struct i2c_client *ov7251;
};

struct surface_camera camera;

static const struct software_node nodes[];

u32 cam3_data_lanes[1] = {1};

static const struct property_entry ov7251_props[] = {
	PROPERTY_ENTRY_U32("clock-frequency", 19200000),
	PROPERTY_ENTRY_U32("bus-type", 5), // D-PHY
	PROPERTY_ENTRY_U32("clock-lanes", 0),
	PROPERTY_ENTRY_U32_ARRAY("data-lanes", cam3_data_lanes),
	PROPERTY_ENTRY_REF("remote-endpoint", &nodes[SWNODE_CIO2_ENDPOINT0]),
	{ }
};

static const struct property_entry cio2_props[] = {
	PROPERTY_ENTRY_U32_ARRAY("data-lanes", cam3_data_lanes),
	PROPERTY_ENTRY_REF("remote-endpoint", &nodes[SWNODE_OV7251_ENDPOINT0]),
	{ }
};

static const struct software_node nodes[] = {
	{ SURFACE_OV7251_HID },
	{ "port0", &nodes[0] },
	{ "endpoint0", &nodes[1], ov7251_props },
	{ SURFACE_CIO2_HID },
	{ "port2", &nodes[3] },
	{ "endpoint0", &nodes[4], cio2_props },
	{ }
};

static void surface_camera_remove_node(const struct software_node *n)
{
	struct fwnode_handle *fwnode = software_node_fwnode(n);
	if (fwnode)
		fwnode_remove_software_node(fwnode);
}

static void surface_camera_unregister_nodes(void)
{
	surface_camera_remove_node(&nodes[SWNODE_OV7251_ENDPOINT0]);
	surface_camera_remove_node(&nodes[SWNODE_OV7251_PORT0]);
	surface_camera_remove_node(&nodes[SWNODE_OV7251]);
	
	surface_camera_remove_node(&nodes[SWNODE_CIO2_ENDPOINT0]);
	surface_camera_remove_node(&nodes[SWNODE_CIO2_PORT0]);
	surface_camera_remove_node(&nodes[SWNODE_CIO2]);
}

static int surface_camera_check_ov7251(struct device *dev, void *data)
{
	struct i2c_client **client = data;
	struct acpi_device *adev;
	const char *hid;

	adev = ACPI_COMPANION(dev);
	if (!adev)
		return 0;

	hid = acpi_device_hid(adev);

	if (strcmp(hid, SURFACE_OV7251_HID))
		return 0;

	*client = to_i2c_client(dev);
	return 1;

}

static int __init surface_camera_init(void)
{
	struct fwnode_handle *fwnode;
	int ret = 0;

	pr_info("%s() called\n", __func__);

	// Register software nodes
	ret = software_node_register_nodes(nodes);
	if (ret)
		return ret;

	// Find i2c device and add swnode as secondary
	i2c_for_each_dev(&camera.ov7251, surface_camera_check_ov7251);
	if (!camera.ov7251) {
		ret = -EPROBE_DEFER;
		goto out;
	}

	fwnode = software_node_fwnode(&nodes[SWNODE_OV7251]);
	if (!fwnode) {
		ret = -ENODEV;
		goto out;
	}

	fwnode->secondary = ERR_PTR(-ENODEV);
	camera.ov7251->dev.fwnode = fwnode;
	((struct v4l2_subdev *)camera.ov7251->dev.driver_data)->fwnode = fwnode;

	// Find pci device and add swnode as primary
	camera.cio2 = pci_get_device(PCI_VENDOR_ID_INTEL, CIO2_PCI_ID, NULL);
	if (!camera.cio2) {
		ret = -EPROBE_DEFER;
		goto out;
	}

	fwnode = software_node_fwnode(&nodes[SWNODE_CIO2]);
	if (!fwnode) {
		ret = -ENODEV;
		goto out;
	}

	fwnode->secondary = ERR_PTR(-ENODEV);
	camera.cio2->dev.fwnode = fwnode;
	ret = device_reprobe(&camera.cio2->dev);
	if (ret) {
		dev_warn(&camera.cio2->dev, "Reprobing error: %d\n", ret);
		goto out;
	}

out:
	if (ret)
		surface_camera_unregister_nodes();

	return ret;
}

static void __exit surface_camera_exit(void)
{
	pr_info("%s() called\n", __func__);

	pci_dev_put(camera.cio2);
	i2c_unregister_device(camera.ov7251);
	surface_camera_unregister_nodes();
}

module_init(surface_camera_init);
module_exit(surface_camera_exit);

MODULE_LICENSE("GPL v2");
MODULE_ALIAS("acpi*:INT343E:*");
