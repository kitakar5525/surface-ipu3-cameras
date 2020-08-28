// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2019 Intel Corporation.

#include <asm/unaligned.h>
#include <linux/acpi.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-fwnode.h>
#include <uapi/linux/v4l2-controls.h>
#include <uapi/linux/videodev2.h>

#include "ov5693.h"

static const uint32_t ov5693_embedded_effective_size = 28;

/* i2c read/write stuff */
static int ov5693_read_reg(struct i2c_client *client,
			   u16 data_length, u16 reg, u16 *val)
{
	int err;
	struct i2c_msg msg[2];
	unsigned char data[6];

	if (!client->adapter) {
		dev_err(&client->dev, "%s error, no client->adapter\n",
			__func__);
		return -ENODEV;
	}

	if (data_length != OV5693_8BIT && data_length != OV5693_16BIT
					&& data_length != OV5693_32BIT) {
		dev_err(&client->dev, "%s error, invalid data length\n",
			__func__);
		return -EINVAL;
	}

	memset(msg, 0 , sizeof(msg));

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = I2C_MSG_LENGTH;
	msg[0].buf = data;

	/* high byte goes out first */
	data[0] = (u8)(reg >> 8);
	data[1] = (u8)(reg & 0xff);

	msg[1].addr = client->addr;
	msg[1].len = data_length;
	msg[1].flags = I2C_M_RD;
	msg[1].buf = data;

	err = i2c_transfer(client->adapter, msg, 2);
	if (err != 2) {
		if (err >= 0)
			err = -EIO;
		dev_err(&client->dev,
			"read from offset 0x%x error %d", reg, err);
		return err;
	}

	*val = 0;
	/* high byte comes first */
	if (data_length == OV5693_8BIT)
		*val = (u8)data[0];
	else if (data_length == OV5693_16BIT)
		*val = be16_to_cpu(*(u16 *)&data[0]);
	else
		*val = be32_to_cpu(*(u32 *)&data[0]);

	return 0;
}

static int ov5693_i2c_write(struct i2c_client *client, u16 len, u8 *data)
{
	struct i2c_msg msg;
	const int num_msg = 1;
	int ret;

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = len;
	msg.buf = data;
	ret = i2c_transfer(client->adapter, &msg, 1);

	return ret == num_msg ? 0 : -EIO;
}

static int ov5693_write_reg(struct i2c_client *client, u16 data_length,
							u16 reg, u16 val)
{
	int ret;
	unsigned char data[4] = {0};
	u16 *wreg = (u16 *)data;
	const u16 len = data_length + sizeof(u16); /* 16-bit address + data */

	if (data_length != OV5693_8BIT && data_length != OV5693_16BIT) {
		dev_err(&client->dev,
			"%s error, invalid data_length\n", __func__);
		return -EINVAL;
	}

	/* high byte goes out first */
	*wreg = cpu_to_be16(reg);

	if (data_length == OV5693_8BIT) {
		data[2] = (u8)(val);
	} else {
		/* OV5693_16BIT */
		u16 *wdata = (u16 *)&data[2];
		*wdata = cpu_to_be16(val);
	}

	ret = ov5693_i2c_write(client, len, data);
	if (ret)
		dev_err(&client->dev,
			"write error: wrote 0x%x to offset 0x%x error %d",
			val, reg, ret);

	return ret;
}

static int ov5693_write_reg_array(struct i2c_client *client,
			      const struct ov5693_reg *regs)
{
	u32 i;
	int ret = 0;

	for (i = 0; ret == 0 && regs[i].type != OV5693_TOK_TERM; i++)
		ret = ov5693_write_reg(client, regs[i].type, regs[i].reg, regs[i].val);

	return ret;
}

static int ov5693_s_ctrl(struct v4l2_ctrl *ctrl)
{
	/*struct ov5693_device *dev =*/
		/*container_of(ctrl->handler, struct ov5693_device, ctrl_handler);*/
	/*struct i2c_client *client = v4l2_get_subdevdata(&dev->sd);*/
	int ret = 0;

	switch (ctrl->id) {
	default:
		ret = -EINVAL;
	}
	return ret;
}

static const struct v4l2_ctrl_ops ctrl_ops = {
	.s_ctrl = ov5693_s_ctrl,
};

static int ov5693_set_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *format)
{
	struct v4l2_mbus_framefmt *fmt = &format->format;
	struct ov5693_device *dev = to_ov5693_sensor(sd);
	struct ov5693_resolution *mode;
	int ret = 0;

	mutex_lock(&dev->input_lock);

	mode = v4l2_find_nearest_size(ov5693_res,
				      N_RES,
				      width, height,
				      fmt->width, fmt->height);
	fmt->width = mode->width;
	fmt->height = mode->height;
	fmt->code = MEDIA_BUS_FMT_SBGGR10_1X10;
	fmt->field = V4L2_FIELD_NONE;

	if (format->which == V4L2_SUBDEV_FORMAT_TRY) {
		*v4l2_subdev_get_try_format(sd, cfg, format->pad) = format->format;
		goto out;
	} else {
		dev->curr_mode = mode;
	}


	/*
	 * After sensor settings are set to HW, sometimes stream is started.
	 * This would cause ISP timeout because ISP is not ready to receive
	 * data yet. So add stop streaming here.
	 */
	/*ret = ov5693_write_reg(client, OV5693_8BIT, OV5693_SW_STREAM,*/
				/*OV5693_STOP_STREAMING);*/
	/*if (ret) {*/
		/*dev_warn(&client->dev, "ov5693 stream off err\n");*/
		/*goto out;*/
	/*}*/

out:
	mutex_unlock(&dev->input_lock);
	return ret;
}
static int ov5693_get_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *format)
{
	struct ov5693_device *dev = to_ov5693_sensor(sd);
	if (format->pad)
		return -EINVAL;

	if (format->which == V4L2_SUBDEV_FORMAT_TRY) {
		format->format = *v4l2_subdev_get_try_format(&dev->sd, cfg,
							  format->pad);
	} else {
		format->format.width = dev->curr_mode->width;
		format->format.height = dev->curr_mode->height;
		format->format.code = MEDIA_BUS_FMT_SBGGR10_1X10;
		format->format.field = V4L2_FIELD_NONE;
	}

	return 0;
}

static int ov5693_check_sensor_id(struct i2c_client *client)
{
	struct i2c_adapter *adapter = client->adapter;
	u16 high, low;
	int ret;
	u16 id;
	u8 revision;

	if (!i2c_check_functionality(adapter, I2C_FUNC_I2C))
		return -ENODEV;

	ret = ov5693_read_reg(client, OV5693_8BIT,
					OV5693_SC_CMMN_CHIP_ID_H, &high);
	if (ret) {
		dev_err(&client->dev, "sensor_id_high = 0x%x\n", high);
		return -ENODEV;
	}
	ret = ov5693_read_reg(client, OV5693_8BIT,
					OV5693_SC_CMMN_CHIP_ID_L, &low);
	id = ((((u16) high) << 8) | (u16) low);

	if (id != OV5693_ID) {
		dev_err(&client->dev, "sensor ID error 0x%x\n", id);
		return -ENODEV;
	}

	ret = ov5693_read_reg(client, OV5693_8BIT,
					OV5693_SC_CMMN_SUB_ID, &high);
	revision = (u8) high & 0x0f;

	dev_dbg(&client->dev, "sensor_revision = 0x%x\n", revision);
	dev_dbg(&client->dev, "detect ov5693 success\n");
	return 0;
}

static int ov5693_stop_streaming(struct ov5693_device *dev)
{
	struct i2c_client *client = v4l2_get_subdevdata(&dev->sd);
	int ret;

	ret = ov5693_write_reg(client, OV5693_8BIT, OV5693_SW_STREAM,
				OV5693_STOP_STREAMING);
	if (ret)
	{
		dev_err(&client->dev, "Failed to stop stream\n");
		return ret;
	}

	return 0;
}

static int ov5693_start_streaming(struct ov5693_device *dev)
{
	struct i2c_client *client = v4l2_get_subdevdata(&dev->sd);
	int ret;
	u16 stream = 0xFF;

	/* Get out of software reset */
	ret = ov5693_write_reg(client, OV5693_8BIT, OV5693_SW_RESET, 0x1);
	if (ret) {
		dev_err(&client->dev, "Failed to set powerup registers\n");
		return ret;
	}	

	ret = ov5693_write_reg_array(client, ov5693_global_setting);
	if (ret) {
		dev_err(&client->dev, "%s failed to set global regs\n", __func__);
		return ret;
	}

	// TODO: Set the PLL registers. See ov5693 DS to get config
	// for 24MHz.
	// Additionally, it's worth reading a couple values over
	// i2c and checking the current value to figure out the
	// XVCLK frequency. IDK maybe not, I don't know if these
	// registers are volatile. I guess the CHIP ID is non-volatile
	// though...
	// Some of these registers seem to be set in global settings?
	ret = ov5693_write_reg_array(client, ov5693_pll_config);
	if (ret) {
		dev_err(&client->dev, "%s failed to set pll config\n", __func__);
		return ret;
	}

	/* Apply default values of current mode */
	ret = ov5693_write_reg_array(client, dev->curr_mode->regs);
	if (ret) {
		dev_err(&client->dev, "%s failed to set mode\n", __func__);
		return ret;
	}

	ret = ov5693_write_reg(client, OV5693_8BIT, OV5693_SW_STREAM,
				OV5693_START_STREAMING);
	if (ret) {
		dev_err(&client->dev, "Failed to set stream\n");
		return ret;
	}

	ret = ov5693_read_reg(client, OV5693_8BIT, OV5693_SW_STREAM,
				&stream);
	if (ret) {
		dev_err(&client->dev, "Failed to get stream\n");
		return ret;
	}

	return 0;
}

static int ov5693_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct ov5693_device *dev = to_ov5693_sensor(sd);
	int ret;

	mutex_lock(&dev->input_lock);

	if (enable) {
		ret = pm_runtime_get_sync(&client->dev);
		if (ret < 0) {
			pm_runtime_put_noidle(&client->dev);
			goto out;
		}

		ret = ov5693_start_streaming(dev);
		if (ret)
			goto out;
	} else {
		ret = ov5693_stop_streaming(dev);
		pm_runtime_put(&client->dev);
	}

out:
	mutex_unlock(&dev->input_lock);

	return ret;
}


static int ov5693_g_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *interval)
{
	struct ov5693_device *dev = to_ov5693_sensor(sd);

	interval->interval.numerator = 1;
	interval->interval.denominator = dev->curr_mode->fps;

	return 0;
}

static int ov5693_enum_mbus_code(struct v4l2_subdev *sd,
				 struct v4l2_subdev_pad_config *cfg,
				 struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->index >= MAX_FMTS)
		return -EINVAL;

	code->code = MEDIA_BUS_FMT_SBGGR10_1X10;
	return 0;
}

static int ov5693_enum_frame_size(struct v4l2_subdev *sd,
				  struct v4l2_subdev_pad_config *cfg,
				  struct v4l2_subdev_frame_size_enum *fse)
{
	int index = fse->index;

	if (index >= N_RES)
		return -EINVAL;

	fse->min_width = ov5693_res[index].width;
	fse->min_height = ov5693_res[index].height;
	fse->max_width = ov5693_res[index].width;
	fse->max_height = ov5693_res[index].height;

	return 0;

}

static const struct v4l2_subdev_video_ops ov5693_video_ops = {
	.s_stream = ov5693_s_stream,
	.g_frame_interval = ov5693_g_frame_interval,
};

static const struct v4l2_subdev_pad_ops ov5693_pad_ops = {
	.enum_mbus_code = ov5693_enum_mbus_code,
	.enum_frame_size = ov5693_enum_frame_size,
	.get_fmt = ov5693_get_fmt,
	.set_fmt = ov5693_set_fmt,
};

static const struct v4l2_subdev_ops ov5693_ops = {
	.video = &ov5693_video_ops,
	.pad = &ov5693_pad_ops,
};

static const struct media_entity_operations ov5693_subdev_entity_ops = {
	.link_validate = v4l2_subdev_link_validate,
};

static int match_depend(struct device *dev, const void *data)
{
	return (dev && dev->fwnode == data) ? 1 : 0;
}

static int __ov5693_power_off(struct ov5693_device *ov5693)
{
	gpiod_set_value_cansleep(ov5693->xshutdn, 0);
	gpiod_set_value_cansleep(ov5693->pwdnb, 0);
	gpiod_set_value_cansleep(ov5693->led_gpio, 0);

	// TODO: I think these could be device managed
	// TODO: Only call put if they were initialized in the first place?
	gpiod_put(ov5693->xshutdn);
	gpiod_put(ov5693->pwdnb);
	gpiod_put(ov5693->led_gpio);

	regulator_bulk_disable(OV5693_NUM_SUPPLIES, ov5693->supplies);

	return 0;
}

static int __ov5693_power_on(struct ov5693_device *ov5693)
{
	struct i2c_client *client = v4l2_get_subdevdata(&ov5693->sd);
	struct acpi_handle *dev_handle = ACPI_HANDLE(&client->dev);
	struct acpi_handle_list dep_devices;
	acpi_status status;
	struct device *dev;
	int ret;
	int i;

	// TODO: Refactor this into own function
	// Get dependant INT3472 device
	if (!acpi_has_method(dev_handle, "_DEP")) {
		printk("No dependant devices\n");
		return -100;
	}

	status = acpi_evaluate_reference(dev_handle, "_DEP", NULL,
					 &dep_devices);
	if (ACPI_FAILURE(status)) {
		printk("Failed to evaluate _DEP.\n");
		return -ENODEV;
	}

	for (i = 0; i < dep_devices.count; i++) {
		struct acpi_device *device;
		struct acpi_device_info *info;

		status = acpi_get_object_info(dep_devices.handles[i], &info);
		if (ACPI_FAILURE(status)) {
			printk("Error reading _DEP device info\n");
			return -ENODEV;
		}

		if (info->valid & ACPI_VALID_HID &&
				!strcmp(info->hardware_id.string, "INT3472")) {
			if (acpi_bus_get_device(dep_devices.handles[i], &device))
				return -ENODEV;

			dev = bus_find_device(&platform_bus_type, NULL,
					&device->fwnode, match_depend);
			if (dev) {
				dev_info(&client->dev, "Dependent platform device found %s\n",
					dev_name(dev));
				break;
			}
		}
	}

	ov5693->xshutdn = gpiod_get_index(dev, NULL, 0, GPIOD_ASIS);
	if (IS_ERR(ov5693->xshutdn)) {
		printk("Couldn't get GPIO XSHUTDN\n");
		return -EINVAL;
	}

	ov5693->pwdnb = gpiod_get_index(dev, NULL, 1, GPIOD_ASIS);
	if (IS_ERR(ov5693->pwdnb)) {
		printk("Couldn't get GPIO PWDNB\n");
		return -EINVAL;
	}

	ov5693->led_gpio = gpiod_get_index(dev, NULL, 2, GPIOD_ASIS);
	if (IS_ERR(ov5693->led_gpio)) {
		printk("Couldn't get GPIO 2\n");
		return -EINVAL;
	}

	gpiod_set_value_cansleep(ov5693->xshutdn, 0);
	gpiod_set_value_cansleep(ov5693->pwdnb, 0);

	ret = regulator_bulk_enable(OV5693_NUM_SUPPLIES, ov5693->supplies);
	if (ret) {
		dev_err(&client->dev, "Failed to enable regulators\n");
		return ret;
	}

	gpiod_set_value_cansleep(ov5693->xshutdn, 0);

	/* according to DS, at least 5ms is needed between DOVDD and PWDN */
	/* add this delay time to 10~11ms*/
	usleep_range(10000, 11000);

	// TODO: According to DS, only one of these is actually controlled by GPIO.
	gpiod_set_value_cansleep(ov5693->xshutdn, 1);
	gpiod_set_value_cansleep(ov5693->pwdnb, 1);

	// TODO: This doesn't really need to be here.
	// Setting this to 1 is mostly just fun cause you can make a light turn on :)
	/*gpiod_set_value_cansleep(ov5693->led_gpio, 1);*/

	// TODO: I don't know if this is actually needed
	usleep_range(10000, 11000);
	
	return 0;
}

static int ov5693_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct ov5693_device *dev = to_ov5693_sensor(sd);
	dev_dbg(&client->dev, "ov5693_remove...\n");

	pm_runtime_disable(&client->dev);

	__ov5693_power_off(dev);

	v4l2_async_unregister_subdev(sd);

	media_entity_cleanup(&dev->sd.entity);
	v4l2_ctrl_handler_free(&dev->ctrl_handler);
	kfree(dev);

	return 0;
}

// TODO: Put these in header?
#define OV5693_LINK_FREQ_19MHZ		19200000
#define OV5693_PIXEL_RATE			(OV5693_LINK_FREQ_19MHZ * 2 * 2) / 10
static const s64 link_freq_menu_items[] = {
	OV5693_LINK_FREQ_19MHZ
};

/**
 * Initialize controls:
 * - link frequency
 * - pixel rate
 *
 * TODO: At least 6 more controls are supported
 * - hblank
 * - vblank
 * - analog gain
 * - exposure
 *
 */
static int ov5693_init_controls(struct ov5693_device *dev)
{
	struct v4l2_ctrl_handler *handler;
	struct i2c_client *client = v4l2_get_subdevdata(&dev->sd);
	struct v4l2_ctrl *ctrl;
	int ret;

	handler = &dev->ctrl_handler;
	ret = v4l2_ctrl_handler_init(handler, 2);
	if (ret)
		return ret;

	handler->lock = &dev->input_lock;

	ctrl = v4l2_ctrl_new_int_menu(handler, NULL, V4L2_CID_LINK_FREQ,
				      0, 0, link_freq_menu_items);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_READ_ONLY;

	v4l2_ctrl_new_std(handler, NULL, V4L2_CID_PIXEL_RATE,
			  0, OV5693_PIXEL_RATE, 1, OV5693_PIXEL_RATE);

	if (handler->error) {
		ret = handler->error;
		dev_err(&client->dev, "Failed to init controls(%d)\n", ret);
		goto err;
	}

	dev->sd.ctrl_handler = handler;

	return 0;

err:
	v4l2_ctrl_handler_free(handler);
	return ret;
}

static int ov5693_configure_regulators(struct ov5693_device *dev)
{
	struct i2c_client *client = v4l2_get_subdevdata(&dev->sd);
	int i;

	for (i = 0; i < OV5693_NUM_SUPPLIES; i++)
		dev->supplies[i].supply = ov5693_supply_names[i];

	return devm_regulator_bulk_get(&client->dev,
				       OV5693_NUM_SUPPLIES,
				       dev->supplies);
}

static int ov5693_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct ov5693_device *dev;
	int ret = 0;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev) {
		dev_err(&client->dev, "out of memory\n");
		ret = -ENOMEM;
		goto out;
	}

	mutex_init(&dev->input_lock);

	v4l2_i2c_subdev_init(&dev->sd, client, &ov5693_ops);

	dev->curr_mode = &ov5693_res[0];
	dev->sd.fwnode = client->dev.fwnode;
	dev->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	dev->format.code = MEDIA_BUS_FMT_SBGGR10_1X10;
	dev->sd.entity.function = MEDIA_ENT_F_CAM_SENSOR;
	dev->sd.entity.ops = &ov5693_subdev_entity_ops;

	ret = ov5693_configure_regulators(dev);
	if (ret) {
		dev_err(&client->dev, "Failed to get power regulators\n");
		return ret;
	}

	// Power on
	ret = __ov5693_power_on(dev);
	if (ret)
	{
		dev_err(&client->dev, "could not power on ov5693\n");
		goto out;
	}

	ret = ov5693_check_sensor_id(client);
	if (ret)
		goto out;

	ret = ov5693_init_controls(dev);
	if (ret)
		goto out;

	dev->pad.flags = MEDIA_PAD_FL_SOURCE;
	ret = media_entity_pads_init(&dev->sd.entity, 1, &dev->pad);
	if (ret)
		goto out;

	// TODO: Consider using sensor_common to also use LEDs
	/*ret = v4l2_async_register_subdev_sensor_common(&dev->sd);*/
	ret = v4l2_async_register_subdev(&dev->sd);
	if (ret) {
		dev_err(&client->dev, "failed to register V4L2 subdev: %d",
			ret);
		goto out;
	}

	pm_runtime_set_active(&client->dev);
	pm_runtime_enable(&client->dev);
	pm_runtime_idle(&client->dev);

out:
	if (ret)
		ov5693_remove(client);

	return ret;
}

static int __maybe_unused ov5693_suspend(struct device *dev)
{
	return 0;
}

static int __maybe_unused ov5693_resume(struct device *dev)
{
	return 0;
}

#ifdef CONFIG_ACPI
static const struct acpi_device_id ov5693_acpi_ids[] = {
	{"INT33BE"},
	{}
};

MODULE_DEVICE_TABLE(acpi, ov5693_acpi_ids);
#endif

static const struct dev_pm_ops ov5693_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(ov5693_suspend, ov5693_resume)
};

static struct i2c_driver ov5693_i2c_driver = {
	.driver = {
		.name = "ov5693",
		.pm = &ov5693_pm_ops,
		.acpi_match_table = ACPI_PTR(ov5693_acpi_ids),
	},
	.probe = ov5693_probe,
	.remove = ov5693_remove,
};

module_i2c_driver(ov5693_i2c_driver);

MODULE_AUTHOR("Jordan Hand <jorhand@linux.microsoft.com>");
MODULE_DESCRIPTION("A low-level driver for OmniVision 5693 sensors");
MODULE_LICENSE("GPL");
