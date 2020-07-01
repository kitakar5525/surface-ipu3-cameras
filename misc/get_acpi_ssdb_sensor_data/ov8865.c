#include <linux/acpi.h>
#include <linux/i2c.h>

/* Data representation as it is in ACPI SSDB buffer */
struct sensor_bios_data_packed {
	u8 version;
	u8 sku;
	u8 guid_csi2[16];
	u8 devfunction;
	u8 bus;
	u32 dphylinkenfuses;
	u32 clockdiv;
	u8 link;
	u8 lanes;
	u32 csiparams[10];
	u32 maxlanespeed;
	u8 sensorcalibfileidx;
	u8 sensorcalibfileidxInMBZ[3];
	u8 romtype;
	u8 vcmtype;
	u8 platforminfo;
	u8 platformsubinfo;
	u8 flash;
	u8 privacyled;
	u8 degree;
	u8 mipilinkdefined;
	u32 mclkspeed;
	u8 controllogicid;
	u8 reserved1[3];
	u8 mclkport;
	u8 reserved2[13];
} __attribute__((__packed__));

/* Fields needed by ipu4 driver */
struct sensor_bios_data {
	struct device *dev;
	u8 link;
	u8 lanes;
	u8 vcmtype;
	u8 flash;
	u8 degree;
	u8 mclkport;
	u16 xshutdown;
};

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

static int get_acpi_ssdb_sensor_data(struct device *dev,
				     struct sensor_bios_data *sensor)
{
	struct sensor_bios_data_packed sensor_data;
	int ret = read_acpi_block(dev, "SSDB", &sensor_data,
				  sizeof(sensor_data));
	if (ret < 0)
		return ret;

	// get_acpi_dep_data(dev, sensor);

	/* Xshutdown is not part of the ssdb data */
	sensor->link = sensor_data.link;
	sensor->lanes = sensor_data.lanes;
	sensor->mclkport = sensor_data.mclkport;
	sensor->flash = sensor_data.flash;
	dev_info(dev, "sensor acpi data: link %d, lanes %d, mclk %d, flash %d\n",
		sensor->link, sensor->lanes, sensor->mclkport, sensor->flash);

	// print additional data
	dev_info(dev, "sensor_data.romtype: %d\n", sensor_data.romtype);
	dev_info(dev, "sensor_data.vcmtype: %d\n", sensor_data.vcmtype);
	dev_info(dev, "sensor_data.privacyled: %d\n", sensor_data.privacyled);
	dev_info(dev, "sensor_data.degree: %d\n", sensor_data.degree);
	dev_info(dev, "sensor_data.mipilinkdefined: %d\n", sensor_data.mipilinkdefined);
	dev_info(dev, "sensor_data.mclkspeed: %d\n", sensor_data.mclkspeed);
	dev_info(dev, "sensor_data.controllogicid: %d\n", sensor_data.controllogicid);

	return 0;
}

static int ov8865_probe(struct i2c_client *client)
{
	struct sensor_bios_data sensor;

	// ov8865 i2c-INT347A:00: sensor acpi data: link 0, lanes 4, mclk 0, flash 2
	// ov8865 i2c-INT347A:00: sensor_data.mclkspeed: 19200000
	get_acpi_ssdb_sensor_data(&client->dev, &sensor);

	return 0;
}

static int ov8865_remove(struct i2c_client *client)
{
	return 0;
}

static const struct acpi_device_id ov8865_acpi_ids[] = {
	{"INT347A"},
	{},
};
MODULE_DEVICE_TABLE(acpi, ov8865_acpi_ids);

static struct i2c_driver ov8865_i2c_driver = {
	.driver = {
		.acpi_match_table = ACPI_PTR(ov8865_acpi_ids),
		.name  = "ov8865",
	},
	.probe_new  = ov8865_probe,
	.remove = ov8865_remove,
};

module_i2c_driver(ov8865_i2c_driver);

MODULE_LICENSE("GPL v2");