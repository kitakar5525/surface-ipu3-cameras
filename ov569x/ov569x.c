// SPDX-License-Identifier: GPL-2.0
/*
 * ov569x driver
 *
 * Copyright (C) 2017 Fuzhou Rockchip Electronics Co., Ltd.
 */

#define OV569X_ACPI_HID "INT33BE"

#include <linux/acpi.h>
#include <linux/clk.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/pm_runtime.h>
#include <linux/regulator/consumer.h>
#include <linux/sysfs.h>
#include <media/media-entity.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-subdev.h>

#ifndef V4L2_CID_DIGITAL_GAIN
#define V4L2_CID_DIGITAL_GAIN		V4L2_CID_GAIN
#endif

/* 45Mhz * 4 Binning */
#define OV569X_PIXEL_RATE		(45 * 1000 * 1000 * 4)
#define OV569X_XVCLK_FREQ		24000000

#define CHIP_ID_OV5693			0x56906c
#define CHIP_ID_OV5695			0x005695
#define OV569X_REG_CHIP_ID		0x300a

#define OV569X_REG_CTRL_MODE		0x0100
#define OV569X_MODE_SW_STANDBY		0x0
#define OV569X_MODE_STREAMING		BIT(0)

#define OV569X_REG_EXPOSURE		0x3500
#define	OV569X_EXPOSURE_MIN		4
#define	OV569X_EXPOSURE_STEP		1
#define OV569X_VTS_MAX			0x7fff

#define OV569X_REG_ANALOG_GAIN		0x3509
#define	ANALOG_GAIN_MIN			0x10
#define	ANALOG_GAIN_MAX			0xf8
#define	ANALOG_GAIN_STEP		1
#define	ANALOG_GAIN_DEFAULT		0xf8

#define OV569X_REG_DIGI_GAIN_H		0x350a
#define OV569X_REG_DIGI_GAIN_L		0x350b
#define OV569X_DIGI_GAIN_L_MASK		0x3f
#define OV569X_DIGI_GAIN_H_SHIFT	6
#define OV569X_DIGI_GAIN_MIN		0
#define OV569X_DIGI_GAIN_MAX		(0x4000 - 1)
#define OV569X_DIGI_GAIN_STEP		1
#define OV569X_DIGI_GAIN_DEFAULT	1024

#define OV569X_REG_TEST_PATTERN		0x4503
#define	OV569X_TEST_PATTERN_ENABLE	0x80
#define	OV569X_TEST_PATTERN_DISABLE	0x0

#define OV569X_REG_VTS			0x380e

#define REG_NULL			0xFFFF

#define OV569X_REG_VALUE_08BIT		1
#define OV569X_REG_VALUE_16BIT		2
#define OV569X_REG_VALUE_24BIT		3

#define OV569X_LANES			2
#define OV569X_BITS_PER_SAMPLE		10

static const char * const ov569x_supply_names[] = {
	"avdd",		/* Analog power */
	"dovdd",	/* Digital I/O power */
	"dvdd",		/* Digital core power */
};

#define OV569X_NUM_SUPPLIES ARRAY_SIZE(ov569x_supply_names)

struct regval {
	u16 addr;
	u8 val;
};

struct ov569x_mode {
	u32 width;
	u32 height;
	u32 max_fps;
	u32 hts_def;
	u32 vts_def;
	u32 exp_def;
	const struct regval *reg_list;
};

struct ov569x {
	struct i2c_client	*client;

	struct v4l2_subdev	subdev;
	struct media_pad	pad;
	struct v4l2_ctrl_handler ctrl_handler;
	struct v4l2_ctrl	*exposure;
	struct v4l2_ctrl	*anal_gain;
	struct v4l2_ctrl	*digi_gain;
	struct v4l2_ctrl	*hblank;
	struct v4l2_ctrl	*vblank;
	struct v4l2_ctrl	*test_pattern;
	struct mutex		mutex;
	bool			streaming;
	const struct ov569x_mode *cur_mode;

	/* For DT-based systems */
	struct clk		*xvclk;
	struct gpio_desc	*reset_gpio;
	struct regulator_bulk_data supplies[OV569X_NUM_SUPPLIES];

	/* For ACPI-based systems */
	/* dependent device (PMIC) */
	struct device *dep_dev;

	/* GPIOs defined in dep_dev _CRS. The last "led_gpio" may not exist
	 * depending on devices.
	 */
	struct gpio_desc *xshutdn;
	struct gpio_desc *pwdnb;
	struct gpio_desc *led_gpio;

	bool is_acpi_based;
};

#define to_ov569x(sd) container_of(sd, struct ov569x, subdev)

/*
 * Xclk 24Mhz
 * Pclk 45Mhz
 * linelength 672(0x2a0)
 * framelength 2232(0x8b8)
 * grabwindow_width 1296
 * grabwindow_height 972
 * max_framerate 30fps
 * mipi_datarate per lane 840Mbps
 */
static const struct regval ov569x_global_regs[] = {
	{0x0103, 0x01},
	{0x0100, 0x00},
	{0x0300, 0x04},
	{0x0301, 0x00},
	{0x0302, 0x69},
	{0x0303, 0x00},
	{0x0304, 0x00},
	{0x0305, 0x01},
	{0x0307, 0x00},
	{0x030b, 0x00},
	{0x030c, 0x00},
	{0x030d, 0x1e},
	{0x030e, 0x04},
	{0x030f, 0x03},
	{0x0312, 0x01},
	{0x3000, 0x00},
	{0x3002, 0xa1},
	{0x3008, 0x00},
	{0x3010, 0x00},
	{0x3022, 0x51},
	{0x3106, 0x15},
	{0x3107, 0x01},
	{0x3108, 0x05},
	{0x3500, 0x00},
	{0x3501, 0x45},
	{0x3502, 0x00},
	{0x3503, 0x08},
	{0x3504, 0x03},
	{0x3505, 0x8c},
	{0x3507, 0x03},
	{0x3508, 0x00},
	{0x3509, 0x10},
	{0x350c, 0x00},
	{0x350d, 0x80},
	{0x3510, 0x00},
	{0x3511, 0x02},
	{0x3512, 0x00},
	{0x3601, 0x55},
	{0x3602, 0x58},
	{0x3614, 0x30},
	{0x3615, 0x77},
	{0x3621, 0x08},
	{0x3624, 0x40},
	{0x3633, 0x0c},
	{0x3634, 0x0c},
	{0x3635, 0x0c},
	{0x3636, 0x0c},
	{0x3638, 0x00},
	{0x3639, 0x00},
	{0x363a, 0x00},
	{0x363b, 0x00},
	{0x363c, 0xff},
	{0x363d, 0xfa},
	{0x3650, 0x44},
	{0x3651, 0x44},
	{0x3652, 0x44},
	{0x3653, 0x44},
	{0x3654, 0x44},
	{0x3655, 0x44},
	{0x3656, 0x44},
	{0x3657, 0x44},
	{0x3660, 0x00},
	{0x3661, 0x00},
	{0x3662, 0x00},
	{0x366a, 0x00},
	{0x366e, 0x0c},
	{0x3673, 0x04},
	{0x3700, 0x14},
	{0x3703, 0x0c},
	{0x3715, 0x01},
	{0x3733, 0x10},
	{0x3734, 0x40},
	{0x373f, 0xa0},
	{0x3765, 0x20},
	{0x37a1, 0x1d},
	{0x37a8, 0x26},
	{0x37ab, 0x14},
	{0x37c2, 0x04},
	{0x37cb, 0x09},
	{0x37cc, 0x13},
	{0x37cd, 0x1f},
	{0x37ce, 0x1f},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x00},
	{0x3803, 0x00},
	{0x3804, 0x0a},
	{0x3805, 0x3f},
	{0x3806, 0x07},
	{0x3807, 0xaf},
	{0x3808, 0x05},
	{0x3809, 0x10},
	{0x380a, 0x03},
	{0x380b, 0xcc},
	{0x380c, 0x02},
	{0x380d, 0xa0},
	{0x380e, 0x08},
	{0x380f, 0xb8},
	{0x3810, 0x00},
	{0x3811, 0x06},
	{0x3812, 0x00},
	{0x3813, 0x06},
	{0x3814, 0x03},
	{0x3815, 0x01},
	{0x3816, 0x03},
	{0x3817, 0x01},
	{0x3818, 0x00},
	{0x3819, 0x00},
	{0x381a, 0x00},
	{0x381b, 0x01},
	{0x3820, 0x8b},
	{0x3821, 0x01},
	{0x3c80, 0x08},
	{0x3c82, 0x00},
	{0x3c83, 0x00},
	{0x3c88, 0x00},
	{0x3d85, 0x14},
	{0x3f02, 0x08},
	{0x3f03, 0x10},
	{0x4008, 0x02},
	{0x4009, 0x09},
	{0x404e, 0x20},
	{0x4501, 0x00},
	{0x4502, 0x10},
	{0x4800, 0x00},
	{0x481f, 0x2a},
	{0x4837, 0x13},
	{0x5000, 0x17},
	{0x5780, 0x3e},
	{0x5781, 0x0f},
	{0x5782, 0x44},
	{0x5783, 0x02},
	{0x5784, 0x01},
	{0x5785, 0x01},
	{0x5786, 0x00},
	{0x5787, 0x04},
	{0x5788, 0x02},
	{0x5789, 0x0f},
	{0x578a, 0xfd},
	{0x578b, 0xf5},
	{0x578c, 0xf5},
	{0x578d, 0x03},
	{0x578e, 0x08},
	{0x578f, 0x0c},
	{0x5790, 0x08},
	{0x5791, 0x06},
	{0x5792, 0x00},
	{0x5793, 0x52},
	{0x5794, 0xa3},
	{0x5b00, 0x00},
	{0x5b01, 0x1c},
	{0x5b02, 0x00},
	{0x5b03, 0x7f},
	{0x5b05, 0x6c},
	{0x5e10, 0xfc},
	{0x4010, 0xf1},
	{0x3503, 0x08},
	{0x3505, 0x8c},
	{0x3507, 0x03},
	{0x3508, 0x00},
	{0x3509, 0xf8},
	{REG_NULL, 0x00},
};

/*
 * Xclk 24Mhz
 * Pclk 45Mhz
 * linelength 740(0x2e4)
 * framelength 2024(0x7e8)
 * grabwindow_width 2592
 * grabwindow_height 1944
 * max_framerate 30fps
 * mipi_datarate per lane 840Mbps
 */
static const struct regval ov569x_2592x1944_regs[] = {
	{0x3501, 0x7e},
	{0x366e, 0x18},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x00},
	{0x3803, 0x04},
	{0x3804, 0x0a},
	{0x3805, 0x3f},
	{0x3806, 0x07},
	{0x3807, 0xab},
	{0x3808, 0x0a},
	{0x3809, 0x20},
	{0x380a, 0x07},
	{0x380b, 0x98},
	{0x380c, 0x02},
	{0x380d, 0xe4},
	{0x380e, 0x07},
	{0x380f, 0xe8},
	{0x3811, 0x06},
	{0x3813, 0x08},
	{0x3814, 0x01},
	{0x3816, 0x01},
	{0x3817, 0x01},
	{0x3820, 0x88},
	{0x3821, 0x00},
	{0x4501, 0x00},
	{0x4008, 0x04},
	{0x4009, 0x13},
	{REG_NULL, 0x00},
};

/*
 * Xclk 24Mhz
 * Pclk 45Mhz
 * linelength 672(0x2a0)
 * framelength 2232(0x8b8)
 * grabwindow_width 1920
 * grabwindow_height 1080
 * max_framerate 30fps
 * mipi_datarate per lane 840Mbps
 */
static const struct regval ov569x_1920x1080_regs[] = {
	{0x3501, 0x45},
	{0x366e, 0x18},
	{0x3800, 0x01},
	{0x3801, 0x50},
	{0x3802, 0x01},
	{0x3803, 0xb8},
	{0x3804, 0x08},
	{0x3805, 0xef},
	{0x3806, 0x05},
	{0x3807, 0xf7},
	{0x3808, 0x07},
	{0x3809, 0x80},
	{0x380a, 0x04},
	{0x380b, 0x38},
	{0x380c, 0x02},
	{0x380d, 0xa0},
	{0x380e, 0x08},
	{0x380f, 0xb8},
	{0x3811, 0x06},
	{0x3813, 0x04},
	{0x3814, 0x01},
	{0x3816, 0x01},
	{0x3817, 0x01},
	{0x3820, 0x88},
	{0x3821, 0x00},
	{0x4501, 0x00},
	{0x4008, 0x04},
	{0x4009, 0x13},
	{REG_NULL, 0x00}
};

/*
 * Xclk 24Mhz
 * Pclk 45Mhz
 * linelength 740(0x02e4)
 * framelength 1012(0x03f4)
 * grabwindow_width 1296
 * grabwindow_height 972
 * max_framerate 60fps
 * mipi_datarate per lane 840Mbps
 */
static const struct regval ov569x_1296x972_regs[] = {
	{0x0103, 0x01},
	{0x0100, 0x00},
	{0x0300, 0x04},
	{0x0301, 0x00},
	{0x0302, 0x69},
	{0x0303, 0x00},
	{0x0304, 0x00},
	{0x0305, 0x01},
	{0x0307, 0x00},
	{0x030b, 0x00},
	{0x030c, 0x00},
	{0x030d, 0x1e},
	{0x030e, 0x04},
	{0x030f, 0x03},
	{0x0312, 0x01},
	{0x3000, 0x00},
	{0x3002, 0xa1},
	{0x3008, 0x00},
	{0x3010, 0x00},
	{0x3016, 0x32},
	{0x3022, 0x51},
	{0x3106, 0x15},
	{0x3107, 0x01},
	{0x3108, 0x05},
	{0x3500, 0x00},
	{0x3501, 0x3e},
	{0x3502, 0x00},
	{0x3503, 0x08},
	{0x3504, 0x03},
	{0x3505, 0x8c},
	{0x3507, 0x03},
	{0x3508, 0x00},
	{0x3509, 0x10},
	{0x350c, 0x00},
	{0x350d, 0x80},
	{0x3510, 0x00},
	{0x3511, 0x02},
	{0x3512, 0x00},
	{0x3601, 0x55},
	{0x3602, 0x58},
	{0x3611, 0x58},
	{0x3614, 0x30},
	{0x3615, 0x77},
	{0x3621, 0x08},
	{0x3624, 0x40},
	{0x3633, 0x0c},
	{0x3634, 0x0c},
	{0x3635, 0x0c},
	{0x3636, 0x0c},
	{0x3638, 0x00},
	{0x3639, 0x00},
	{0x363a, 0x00},
	{0x363b, 0x00},
	{0x363c, 0xff},
	{0x363d, 0xfa},
	{0x3650, 0x44},
	{0x3651, 0x44},
	{0x3652, 0x44},
	{0x3653, 0x44},
	{0x3654, 0x44},
	{0x3655, 0x44},
	{0x3656, 0x44},
	{0x3657, 0x44},
	{0x3660, 0x00},
	{0x3661, 0x00},
	{0x3662, 0x00},
	{0x366a, 0x00},
	{0x366e, 0x0c},
	{0x3673, 0x04},
	{0x3700, 0x14},
	{0x3703, 0x0c},
	{0x3706, 0x24},
	{0x3714, 0x27},
	{0x3715, 0x01},
	{0x3716, 0x00},
	{0x3717, 0x02},
	{0x3733, 0x10},
	{0x3734, 0x40},
	{0x373f, 0xa0},
	{0x3765, 0x20},
	{0x37a1, 0x1d},
	{0x37a8, 0x26},
	{0x37ab, 0x14},
	{0x37c2, 0x04},
	{0x37c3, 0xf0},
	{0x37cb, 0x09},
	{0x37cc, 0x13},
	{0x37cd, 0x1f},
	{0x37ce, 0x1f},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x00},
	{0x3803, 0x00},
	{0x3804, 0x0a},
	{0x3805, 0x3f},
	{0x3806, 0x07},
	{0x3807, 0xaf},
	{0x3808, 0x05},
	{0x3809, 0x10},
	{0x380a, 0x03},
	{0x380b, 0xcc},
	{0x380c, 0x02},
	{0x380d, 0xe4},
	{0x380e, 0x03},
	{0x380f, 0xf4},
	{0x3810, 0x00},
	{0x3811, 0x00},
	{0x3812, 0x00},
	{0x3813, 0x06},
	{0x3814, 0x03},
	{0x3815, 0x01},
	{0x3816, 0x03},
	{0x3817, 0x01},
	{0x3818, 0x00},
	{0x3819, 0x00},
	{0x381a, 0x00},
	{0x381b, 0x01},
	{0x3820, 0x8b},
	{0x3821, 0x01},
	{0x3c80, 0x08},
	{0x3c82, 0x00},
	{0x3c83, 0x00},
	{0x3c88, 0x00},
	{0x3d85, 0x14},
	{0x3f02, 0x08},
	{0x3f03, 0x10},
	{0x4008, 0x02},
	{0x4009, 0x09},
	{0x404e, 0x20},
	{0x4501, 0x00},
	{0x4502, 0x10},
	{0x4800, 0x00},
	{0x481f, 0x2a},
	{0x4837, 0x13},
	{0x5000, 0x13},
	{0x5780, 0x3e},
	{0x5781, 0x0f},
	{0x5782, 0x44},
	{0x5783, 0x02},
	{0x5784, 0x01},
	{0x5785, 0x01},
	{0x5786, 0x00},
	{0x5787, 0x04},
	{0x5788, 0x02},
	{0x5789, 0x0f},
	{0x578a, 0xfd},
	{0x578b, 0xf5},
	{0x578c, 0xf5},
	{0x578d, 0x03},
	{0x578e, 0x08},
	{0x578f, 0x0c},
	{0x5790, 0x08},
	{0x5791, 0x06},
	{0x5792, 0x00},
	{0x5793, 0x52},
	{0x5794, 0xa3},
	{0x5b00, 0x00},
	{0x5b01, 0x1c},
	{0x5b02, 0x00},
	{0x5b03, 0x7f},
	{0x5b05, 0x6c},
	{0x5e10, 0xfc},
	{0x4010, 0xf1},
	{0x3503, 0x08},
	{0x3505, 0x8c},
	{0x3507, 0x03},
	{0x3508, 0x00},
	{0x3509, 0xf8},
	{0x0100, 0x01},
	{REG_NULL, 0x00}
};

/*
 * Xclk 24Mhz
 * Pclk 45Mhz
 * linelength 672(0x2a0)
 * framelength 2232(0x8b8)
 * grabwindow_width 1280
 * grabwindow_height 720
 * max_framerate 30fps
 * mipi_datarate per lane 840Mbps
 */
static const struct regval ov569x_1280x720_regs[] = {
	{0x3501, 0x45},
	{0x366e, 0x0c},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x01},
	{0x3803, 0x00},
	{0x3804, 0x0a},
	{0x3805, 0x3f},
	{0x3806, 0x06},
	{0x3807, 0xaf},
	{0x3808, 0x05},
	{0x3809, 0x00},
	{0x380a, 0x02},
	{0x380b, 0xd0},
	{0x380c, 0x02},
	{0x380d, 0xa0},
	{0x380e, 0x08},
	{0x380f, 0xb8},
	{0x3811, 0x06},
	{0x3813, 0x02},
	{0x3814, 0x03},
	{0x3816, 0x03},
	{0x3817, 0x01},
	{0x3820, 0x8b},
	{0x3821, 0x01},
	{0x4501, 0x00},
	{0x4008, 0x02},
	{0x4009, 0x09},
	{REG_NULL, 0x00}
};

/*
 * Xclk 24Mhz
 * Pclk 45Mhz
 * linelength 672(0x2a0)
 * framelength 558(0x22e)
 * grabwindow_width 640
 * grabwindow_height 480
 * max_framerate 120fps
 * mipi_datarate per lane 840Mbps
 */
static const struct regval ov569x_640x480_regs[] = {
	{0x3501, 0x22},
	{0x366e, 0x0c},
	{0x3800, 0x00},
	{0x3801, 0x00},
	{0x3802, 0x00},
	{0x3803, 0x08},
	{0x3804, 0x0a},
	{0x3805, 0x3f},
	{0x3806, 0x07},
	{0x3807, 0xa7},
	{0x3808, 0x02},
	{0x3809, 0x80},
	{0x380a, 0x01},
	{0x380b, 0xe0},
	{0x380c, 0x02},
	{0x380d, 0xa0},
	{0x380e, 0x02},
	{0x380f, 0x2e},
	{0x3811, 0x06},
	{0x3813, 0x04},
	{0x3814, 0x07},
	{0x3816, 0x05},
	{0x3817, 0x03},
	{0x3820, 0x8d},
	{0x3821, 0x01},
	{0x4501, 0x00},
	{0x4008, 0x02},
	{0x4009, 0x09},
	{REG_NULL, 0x00}
};

static const struct ov569x_mode supported_modes[] = {
	{
		.width = 2592,
		.height = 1944,
		.max_fps = 30,
		.exp_def = 0x0450,
		.hts_def = 0x02e4 * 4,
		.vts_def = 0x07e8,
		.reg_list = ov569x_2592x1944_regs,
	},
	{
		.width = 1920,
		.height = 1080,
		.max_fps = 30,
		.exp_def = 0x0450,
		.hts_def = 0x02a0 * 4,
		.vts_def = 0x08b8,
		.reg_list = ov569x_1920x1080_regs,
	},
	{
		.width = 1296,
		.height = 972,
		.max_fps = 60,
		.exp_def = 0x03e0,
		.hts_def = 0x02e4 * 4,
		.vts_def = 0x03f4,
		.reg_list = ov569x_1296x972_regs,
	},
	{
		.width = 1280,
		.height = 720,
		.max_fps = 30,
		.exp_def = 0x0450,
		.hts_def = 0x02a0 * 4,
		.vts_def = 0x08b8,
		.reg_list = ov569x_1280x720_regs,
	},
	{
		.width = 640,
		.height = 480,
		.max_fps = 120,
		.exp_def = 0x0450,
		.hts_def = 0x02a0 * 4,
		.vts_def = 0x022e,
		.reg_list = ov569x_640x480_regs,
	},
};

#define OV569X_LINK_FREQ_420MHZ		420000000
static const s64 link_freq_menu_items[] = {
	OV569X_LINK_FREQ_420MHZ
};

static const char * const ov569x_test_pattern_menu[] = {
	"Disabled",
	"Vertical Color Bar Type 1",
	"Vertical Color Bar Type 2",
	"Vertical Color Bar Type 3",
	"Vertical Color Bar Type 4"
};

/* Write registers up to 4 at a time */
static int ov569x_write_reg(struct i2c_client *client, u16 reg,
			    u32 len, u32 val)
{
	u32 buf_i, val_i;
	u8 buf[6];
	u8 *val_p;
	__be32 val_be;

	if (len > 4)
		return -EINVAL;

	buf[0] = reg >> 8;
	buf[1] = reg & 0xff;

	val_be = cpu_to_be32(val);
	val_p = (u8 *)&val_be;
	buf_i = 2;
	val_i = 4 - len;

	while (val_i < 4)
		buf[buf_i++] = val_p[val_i++];

	if (i2c_master_send(client, buf, len + 2) != len + 2)
		return -EIO;

	return 0;
}

static int ov569x_write_array(struct i2c_client *client,
			      const struct regval *regs)
{
	u32 i;
	int ret = 0;

	for (i = 0; ret == 0 && regs[i].addr != REG_NULL; i++)
		ret = ov569x_write_reg(client, regs[i].addr,
				       OV569X_REG_VALUE_08BIT, regs[i].val);

	return ret;
}

/* Read registers up to 4 at a time */
static int ov569x_read_reg(struct i2c_client *client, u16 reg, unsigned int len,
			   u32 *val)
{
	struct i2c_msg msgs[2];
	u8 *data_be_p;
	__be32 data_be = 0;
	__be16 reg_addr_be = cpu_to_be16(reg);
	int ret;

	if (len > 4)
		return -EINVAL;

	data_be_p = (u8 *)&data_be;
	/* Write register address */
	msgs[0].addr = client->addr;
	msgs[0].flags = 0;
	msgs[0].len = 2;
	msgs[0].buf = (u8 *)&reg_addr_be;

	/* Read data from register */
	msgs[1].addr = client->addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = len;
	msgs[1].buf = &data_be_p[4 - len];

	ret = i2c_transfer(client->adapter, msgs, ARRAY_SIZE(msgs));
	if (ret != ARRAY_SIZE(msgs))
		return -EIO;

	*val = be32_to_cpu(data_be);

	return 0;
}

static int ov569x_get_reso_dist(const struct ov569x_mode *mode,
				struct v4l2_mbus_framefmt *framefmt)
{
	return abs(mode->width - framefmt->width) +
	       abs(mode->height - framefmt->height);
}

static const struct ov569x_mode *
ov569x_find_best_fit(struct v4l2_subdev_format *fmt)
{
	struct v4l2_mbus_framefmt *framefmt = &fmt->format;
	int dist;
	int cur_best_fit = 0;
	int cur_best_fit_dist = -1;
	int i;

	for (i = 0; i < ARRAY_SIZE(supported_modes); i++) {
		dist = ov569x_get_reso_dist(&supported_modes[i], framefmt);
		if (cur_best_fit_dist == -1 || dist < cur_best_fit_dist) {
			cur_best_fit_dist = dist;
			cur_best_fit = i;
		}
	}

	return &supported_modes[cur_best_fit];
}

static int ov569x_set_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *fmt)
{
	struct ov569x *ov569x = to_ov569x(sd);
	const struct ov569x_mode *mode;
	s64 h_blank, vblank_def;

	mutex_lock(&ov569x->mutex);

	mode = ov569x_find_best_fit(fmt);
	fmt->format.code = MEDIA_BUS_FMT_SBGGR10_1X10;
	fmt->format.width = mode->width;
	fmt->format.height = mode->height;
	fmt->format.field = V4L2_FIELD_NONE;
	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
		*v4l2_subdev_get_try_format(sd, cfg, fmt->pad) = fmt->format;
#endif
	} else {
		ov569x->cur_mode = mode;
		h_blank = mode->hts_def - mode->width;
		__v4l2_ctrl_modify_range(ov569x->hblank, h_blank,
					 h_blank, 1, h_blank);
		vblank_def = mode->vts_def - mode->height;
		__v4l2_ctrl_modify_range(ov569x->vblank, vblank_def,
					 OV569X_VTS_MAX - mode->height,
					 1, vblank_def);
	}

	mutex_unlock(&ov569x->mutex);

	return 0;
}

static int ov569x_get_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *fmt)
{
	struct ov569x *ov569x = to_ov569x(sd);
	const struct ov569x_mode *mode = ov569x->cur_mode;

	mutex_lock(&ov569x->mutex);
	if (fmt->which == V4L2_SUBDEV_FORMAT_TRY) {
#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
		fmt->format = *v4l2_subdev_get_try_format(sd, cfg, fmt->pad);
#else
		mutex_unlock(&ov569x->mutex);
		return -EINVAL;
#endif
	} else {
		fmt->format.width = mode->width;
		fmt->format.height = mode->height;
		fmt->format.code = MEDIA_BUS_FMT_SBGGR10_1X10;
		fmt->format.field = V4L2_FIELD_NONE;
	}
	mutex_unlock(&ov569x->mutex);

	return 0;
}

static int ov569x_enum_mbus_code(struct v4l2_subdev *sd,
				 struct v4l2_subdev_pad_config *cfg,
				 struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->index != 0)
		return -EINVAL;
	code->code = MEDIA_BUS_FMT_SBGGR10_1X10;

	return 0;
}

static int ov569x_enum_frame_sizes(struct v4l2_subdev *sd,
				   struct v4l2_subdev_pad_config *cfg,
				   struct v4l2_subdev_frame_size_enum *fse)
{
	if (fse->index >= ARRAY_SIZE(supported_modes))
		return -EINVAL;

	if (fse->code != MEDIA_BUS_FMT_SBGGR10_1X10)
		return -EINVAL;

	fse->min_width  = supported_modes[fse->index].width;
	fse->max_width  = supported_modes[fse->index].width;
	fse->max_height = supported_modes[fse->index].height;
	fse->min_height = supported_modes[fse->index].height;

	return 0;
}

static int ov569x_enable_test_pattern(struct ov569x *ov569x, u32 pattern)
{
	u32 val;

	if (pattern)
		val = (pattern - 1) | OV569X_TEST_PATTERN_ENABLE;
	else
		val = OV569X_TEST_PATTERN_DISABLE;

	return ov569x_write_reg(ov569x->client, OV569X_REG_TEST_PATTERN,
				OV569X_REG_VALUE_08BIT, val);
}

static int __ov569x_start_stream(struct ov569x *ov569x)
{
	int ret;

	ret = ov569x_write_array(ov569x->client, ov569x_global_regs);
	if (ret)
		return ret;
	ret = ov569x_write_array(ov569x->client, ov569x->cur_mode->reg_list);
	if (ret)
		return ret;

	/* In case these controls are set before streaming */
	ret = __v4l2_ctrl_handler_setup(&ov569x->ctrl_handler);
	if (ret)
		return ret;

	return ov569x_write_reg(ov569x->client, OV569X_REG_CTRL_MODE,
				OV569X_REG_VALUE_08BIT, OV569X_MODE_STREAMING);
}

static int __ov569x_stop_stream(struct ov569x *ov569x)
{
	return ov569x_write_reg(ov569x->client, OV569X_REG_CTRL_MODE,
				OV569X_REG_VALUE_08BIT, OV569X_MODE_SW_STANDBY);
}

static int ov569x_s_stream(struct v4l2_subdev *sd, int on)
{
	struct ov569x *ov569x = to_ov569x(sd);
	struct i2c_client *client = ov569x->client;
	int ret = 0;

	mutex_lock(&ov569x->mutex);
	on = !!on;
	if (on == ov569x->streaming)
		goto unlock_and_return;

	if (on) {
		ret = pm_runtime_get_sync(&client->dev);
		if (ret < 0) {
			pm_runtime_put_noidle(&client->dev);
			goto unlock_and_return;
		}

		ret = __ov569x_start_stream(ov569x);
		if (ret) {
			v4l2_err(sd, "start stream failed while write regs\n");
			pm_runtime_put(&client->dev);
			goto unlock_and_return;
		}
	} else {
		__ov569x_stop_stream(ov569x);
		pm_runtime_put(&client->dev);
	}

	ov569x->streaming = on;

unlock_and_return:
	mutex_unlock(&ov569x->mutex);

	return ret;
}

/* Get GPIOs defined in dep_dev _CRS */
static int gpio_crs_get(struct ov569x *sensor, struct device *dep_dev)
{
	sensor->xshutdn = devm_gpiod_get_index(dep_dev, NULL, 0, GPIOD_ASIS);
	if (IS_ERR(sensor->xshutdn)) {
		dev_err(dep_dev, "Couldn't get GPIO XSHUTDN\n");
		return -EINVAL;
	}

	sensor->pwdnb = devm_gpiod_get_index(dep_dev, NULL, 1, GPIOD_ASIS);
	if (IS_ERR(sensor->pwdnb)) {
		dev_err(dep_dev, "Couldn't get GPIO PWDNB\n");
		return -EINVAL;
	}

	sensor->led_gpio = devm_gpiod_get_index(dep_dev, NULL, 2, GPIOD_ASIS);
	if (IS_ERR(sensor->led_gpio))
		dev_info(dep_dev,
			 "Couldn't get GPIO LED. Maybe not exist, continue anyway.\n");

	return 0;
}

/* Put GPIOs defined in dep_dev _CRS */
static void gpio_crs_put(struct ov569x *sensor)
{
	gpiod_put(sensor->xshutdn);
	gpiod_put(sensor->pwdnb);
	if (!IS_ERR(sensor->led_gpio))
		gpiod_put(sensor->led_gpio);
}

/* Control GPIOs defined in dep_dev _CRS */
static int gpio_crs_ctrl(struct v4l2_subdev *sd, bool flag)
{
	struct ov569x *sensor = to_ov569x(sd);

	gpiod_set_value_cansleep(sensor->xshutdn, flag);
	gpiod_set_value_cansleep(sensor->pwdnb, flag);
	if (!IS_ERR(sensor->led_gpio))
		gpiod_set_value_cansleep(sensor->led_gpio, flag);

	return 0;
}

static int __ov569x_power_on(struct ov569x *ov569x)
{
	int i, ret;
	struct device *dev = &ov569x->client->dev;
	struct v4l2_subdev *sd = i2c_get_clientdata(ov569x->client);

	/* For DT-based systems */
	if (!ov569x->is_acpi_based) {
		ret = clk_prepare_enable(ov569x->xvclk);
		if (ret < 0) {
			dev_err(dev, "Failed to enable xvclk\n");
			return ret;
		}

		gpiod_set_value_cansleep(ov569x->reset_gpio, 1);

		/*
		* The hardware requires the regulators to be powered on in order,
		* so enable them one by one.
		*/
		for (i = 0; i < OV569X_NUM_SUPPLIES; i++) {
			ret = regulator_enable(ov569x->supplies[i].consumer);
			if (ret) {
				dev_err(dev, "Failed to enable %s: %d\n",
					ov569x->supplies[i].supply, ret);
				goto disable_reg_clk;
			}
		}

		gpiod_set_value_cansleep(ov569x->reset_gpio, 0);
	}

	/* For ACPI-based systems */
	if (ov569x->is_acpi_based)
		gpio_crs_ctrl(sd, true);

	usleep_range(1000, 1200);

	return 0;

disable_reg_clk:
	/* For DT-based systems */
	if (!ov569x->is_acpi_based) {
		for (--i; i >= 0; i--)
			regulator_disable(ov569x->supplies[i].consumer);
		clk_disable_unprepare(ov569x->xvclk);
	}

	return ret;
}

static void __ov569x_power_off(struct ov569x *ov569x)
{
	struct device *dev = &ov569x->client->dev;
	struct v4l2_subdev *sd = i2c_get_clientdata(ov569x->client);
	int i, ret;

	/* For DT-based systems */
	if (!ov569x->is_acpi_based) {
		clk_disable_unprepare(ov569x->xvclk);
		gpiod_set_value_cansleep(ov569x->reset_gpio, 1);

		/*
		* The hardware requires the regulators to be powered off in order,
		* so disable them one by one.
		*/
		for (i = OV569X_NUM_SUPPLIES - 1; i >= 0; i--) {
			ret = regulator_disable(ov569x->supplies[i].consumer);
			if (ret)
				dev_err(dev, "Failed to disable %s: %d\n",
					ov569x->supplies[i].supply, ret);
		}
	}

	/* For ACPI-based systems */
	if (ov569x->is_acpi_based)
		gpio_crs_ctrl(sd, false);
}

static int __maybe_unused ov569x_runtime_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct ov569x *ov569x = to_ov569x(sd);

	return __ov569x_power_on(ov569x);
}

static int __maybe_unused ov569x_runtime_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct ov569x *ov569x = to_ov569x(sd);

	__ov569x_power_off(ov569x);

	return 0;
}

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
static int ov569x_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct ov569x *ov569x = to_ov569x(sd);
	struct v4l2_mbus_framefmt *try_fmt =
				v4l2_subdev_get_try_format(sd, fh->pad, 0);
	const struct ov569x_mode *def_mode = &supported_modes[0];

	mutex_lock(&ov569x->mutex);
	/* Initialize try_fmt */
	try_fmt->width = def_mode->width;
	try_fmt->height = def_mode->height;
	try_fmt->code = MEDIA_BUS_FMT_SBGGR10_1X10;
	try_fmt->field = V4L2_FIELD_NONE;

	mutex_unlock(&ov569x->mutex);
	/* No crop or compose */

	return 0;
}
#endif

static const struct dev_pm_ops ov569x_pm_ops = {
	SET_RUNTIME_PM_OPS(ov569x_runtime_suspend,
			   ov569x_runtime_resume, NULL)
};

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
static const struct v4l2_subdev_internal_ops ov569x_internal_ops = {
	.open = ov569x_open,
};
#endif

static const struct v4l2_subdev_video_ops ov569x_video_ops = {
	.s_stream = ov569x_s_stream,
};

static const struct v4l2_subdev_pad_ops ov569x_pad_ops = {
	.enum_mbus_code = ov569x_enum_mbus_code,
	.enum_frame_size = ov569x_enum_frame_sizes,
	.get_fmt = ov569x_get_fmt,
	.set_fmt = ov569x_set_fmt,
};

static const struct v4l2_subdev_ops ov569x_subdev_ops = {
	.video	= &ov569x_video_ops,
	.pad	= &ov569x_pad_ops,
};

static int ov569x_set_ctrl(struct v4l2_ctrl *ctrl)
{
	struct ov569x *ov569x = container_of(ctrl->handler,
					     struct ov569x, ctrl_handler);
	struct i2c_client *client = ov569x->client;
	s64 max;
	int ret = 0;

	/* Propagate change of current control to all related controls */
	switch (ctrl->id) {
	case V4L2_CID_VBLANK:
		/* Update max exposure while meeting expected vblanking */
		max = ov569x->cur_mode->height + ctrl->val - 4;
		__v4l2_ctrl_modify_range(ov569x->exposure,
					 ov569x->exposure->minimum, max,
					 ov569x->exposure->step,
					 ov569x->exposure->default_value);
		break;
	}

	if (!pm_runtime_get_if_in_use(&client->dev))
		return 0;

	switch (ctrl->id) {
	case V4L2_CID_EXPOSURE:
		/* 4 least significant bits of expsoure are fractional part */
		ret = ov569x_write_reg(ov569x->client, OV569X_REG_EXPOSURE,
				       OV569X_REG_VALUE_24BIT, ctrl->val << 4);
		break;
	case V4L2_CID_ANALOGUE_GAIN:
		ret = ov569x_write_reg(ov569x->client, OV569X_REG_ANALOG_GAIN,
				       OV569X_REG_VALUE_08BIT, ctrl->val);
		break;
	case V4L2_CID_DIGITAL_GAIN:
		ret = ov569x_write_reg(ov569x->client, OV569X_REG_DIGI_GAIN_L,
				       OV569X_REG_VALUE_08BIT,
				       ctrl->val & OV569X_DIGI_GAIN_L_MASK);
		ret = ov569x_write_reg(ov569x->client, OV569X_REG_DIGI_GAIN_H,
				       OV569X_REG_VALUE_08BIT,
				       ctrl->val >> OV569X_DIGI_GAIN_H_SHIFT);
		break;
	case V4L2_CID_VBLANK:
		ret = ov569x_write_reg(ov569x->client, OV569X_REG_VTS,
				       OV569X_REG_VALUE_16BIT,
				       ctrl->val + ov569x->cur_mode->height);
		break;
	case V4L2_CID_TEST_PATTERN:
		ret = ov569x_enable_test_pattern(ov569x, ctrl->val);
		break;
	default:
		dev_warn(&client->dev, "%s Unhandled id:0x%x, val:0x%x\n",
			 __func__, ctrl->id, ctrl->val);
		break;
	}

	pm_runtime_put(&client->dev);

	return ret;
}

static const struct v4l2_ctrl_ops ov569x_ctrl_ops = {
	.s_ctrl = ov569x_set_ctrl,
};

static int ov569x_initialize_controls(struct ov569x *ov569x)
{
	const struct ov569x_mode *mode;
	struct v4l2_ctrl_handler *handler;
	struct v4l2_ctrl *ctrl;
	s64 exposure_max, vblank_def;
	u32 h_blank;
	int ret;

	handler = &ov569x->ctrl_handler;
	mode = ov569x->cur_mode;
	ret = v4l2_ctrl_handler_init(handler, 8);
	if (ret)
		return ret;
	handler->lock = &ov569x->mutex;

	ctrl = v4l2_ctrl_new_int_menu(handler, NULL, V4L2_CID_LINK_FREQ,
				      0, 0, link_freq_menu_items);
	if (ctrl)
		ctrl->flags |= V4L2_CTRL_FLAG_READ_ONLY;

	v4l2_ctrl_new_std(handler, NULL, V4L2_CID_PIXEL_RATE,
			  0, OV569X_PIXEL_RATE, 1, OV569X_PIXEL_RATE);

	h_blank = mode->hts_def - mode->width;
	ov569x->hblank = v4l2_ctrl_new_std(handler, NULL, V4L2_CID_HBLANK,
				h_blank, h_blank, 1, h_blank);
	if (ov569x->hblank)
		ov569x->hblank->flags |= V4L2_CTRL_FLAG_READ_ONLY;

	vblank_def = mode->vts_def - mode->height;
	ov569x->vblank = v4l2_ctrl_new_std(handler, &ov569x_ctrl_ops,
				V4L2_CID_VBLANK, vblank_def,
				OV569X_VTS_MAX - mode->height,
				1, vblank_def);

	exposure_max = mode->vts_def - 4;
	ov569x->exposure = v4l2_ctrl_new_std(handler, &ov569x_ctrl_ops,
				V4L2_CID_EXPOSURE, OV569X_EXPOSURE_MIN,
				exposure_max, OV569X_EXPOSURE_STEP,
				mode->exp_def);

	ov569x->anal_gain = v4l2_ctrl_new_std(handler, &ov569x_ctrl_ops,
				V4L2_CID_ANALOGUE_GAIN, ANALOG_GAIN_MIN,
				ANALOG_GAIN_MAX, ANALOG_GAIN_STEP,
				ANALOG_GAIN_DEFAULT);

	/* Digital gain */
	ov569x->digi_gain = v4l2_ctrl_new_std(handler, &ov569x_ctrl_ops,
				V4L2_CID_DIGITAL_GAIN, OV569X_DIGI_GAIN_MIN,
				OV569X_DIGI_GAIN_MAX, OV569X_DIGI_GAIN_STEP,
				OV569X_DIGI_GAIN_DEFAULT);

	ov569x->test_pattern = v4l2_ctrl_new_std_menu_items(handler,
				&ov569x_ctrl_ops, V4L2_CID_TEST_PATTERN,
				ARRAY_SIZE(ov569x_test_pattern_menu) - 1,
				0, 0, ov569x_test_pattern_menu);

	if (handler->error) {
		ret = handler->error;
		dev_err(&ov569x->client->dev,
			"Failed to init controls(%d)\n", ret);
		goto err_free_handler;
	}

	ov569x->subdev.ctrl_handler = handler;

	return 0;

err_free_handler:
	v4l2_ctrl_handler_free(handler);

	return ret;
}

static int ov569x_check_sensor_id(struct ov569x *ov569x,
				  struct i2c_client *client)
{
	struct device *dev = &ov569x->client->dev;
	u32 id = 0;
	int ret;

	ret = ov569x_read_reg(client, OV569X_REG_CHIP_ID,
			      OV569X_REG_VALUE_24BIT, &id);
	switch(id) {
	case CHIP_ID_OV5693:
	case CHIP_ID_OV5695:
		break;
	default:
		dev_err(dev, "Unexpected sensor id(%06x), ret(%d)\n", id, ret);
		return -ENODEV;
	}

	dev_info(dev, "Detected OV%06x sensor\n", id);

	return 0;
}

static int ov569x_configure_regulators(struct ov569x *ov569x)
{
	int i;

	for (i = 0; i < OV569X_NUM_SUPPLIES; i++)
		ov569x->supplies[i].supply = ov569x_supply_names[i];

	return devm_regulator_bulk_get(&ov569x->client->dev,
				       OV569X_NUM_SUPPLIES,
				       ov569x->supplies);
}

/* Get dependent INT3472 device */
static struct device *get_dep_dev(struct device *dev)
{
	struct acpi_handle *dev_handle;
	struct acpi_device *sensor_adev;
	struct acpi_handle_list dep_devices;
	struct acpi_device *dep_adev;
	struct acpi_device_physical_node *dep_phys;
	int ret;
	int i;

	sensor_adev = acpi_dev_get_first_match_dev(OV569X_ACPI_HID, NULL, -1);
	if (!sensor_adev) {
		dev_err(dev, "Couldn't get sensor ACPI device\n");
		return ERR_PTR(-ENODEV);
	}
	dev_handle = sensor_adev->handle;
	acpi_dev_put(sensor_adev);

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

static int ov569x_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct ov569x *ov569x;
	struct v4l2_subdev *sd;
	struct device *dep_dev;
	int ret;

	dev_info(dev, "%s() called\n", __func__);

	ov569x = devm_kzalloc(dev, sizeof(*ov569x), GFP_KERNEL);
	if (!ov569x)
		return -ENOMEM;

	ov569x->client = client;
	ov569x->cur_mode = &supported_modes[0];

	if (acpi_dev_present(OV569X_ACPI_HID, NULL, -1)) {
		dev_info(dev, "system is acpi-based\n");
		ov569x->is_acpi_based = true;
	} else {
		dev_info(dev, "system is not acpi-based\n");
	}

	/* For DT-based systems */
	if (!ov569x->is_acpi_based) {
		ov569x->xvclk = devm_clk_get(dev, "xvclk");
		if (IS_ERR(ov569x->xvclk)) {
			dev_err(dev, "Failed to get xvclk\n");
			return -EINVAL;
		}
		ret = clk_set_rate(ov569x->xvclk, OV569X_XVCLK_FREQ);
		if (ret < 0) {
			dev_err(dev, "Failed to set xvclk rate (24MHz)\n");
			return ret;
		}
		if (clk_get_rate(ov569x->xvclk) != OV569X_XVCLK_FREQ)
			dev_warn(dev, "xvclk mismatched, modes are based on 24MHz\n");

		ov569x->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
		if (IS_ERR(ov569x->reset_gpio)) {
			dev_err(dev, "Failed to get reset-gpios\n");
			return -EINVAL;
		}

		ret = ov569x_configure_regulators(ov569x);
		if (ret) {
			dev_err(dev, "Failed to get power regulators\n");
			return ret;
		}
	}

	/* For ACPI-based systems */
	if (ov569x->is_acpi_based) {
		ov569x->dep_dev = get_dep_dev(dev);
		if (IS_ERR(ov569x->dep_dev)) {
			ret = PTR_ERR(ov569x->dep_dev);
			dev_err(dev, "cannot get dep_dev: ret %d\n", ret);
			return ret;
		}
		dep_dev = ov569x->dep_dev;

		ret = gpio_crs_get(ov569x, dep_dev);
		if (ret) {
			dev_err(dep_dev, "Failed to get _CRS GPIOs\n");
			return ret;
		}
	}

	mutex_init(&ov569x->mutex);

	sd = &ov569x->subdev;
	v4l2_i2c_subdev_init(sd, client, &ov569x_subdev_ops);
	ret = ov569x_initialize_controls(ov569x);
	if (ret)
		goto err_destroy_mutex;

	ret = __ov569x_power_on(ov569x);
	if (ret)
		goto err_free_handler;

	ret = ov569x_check_sensor_id(ov569x, client);
	if (ret)
		goto err_power_off;

#ifdef CONFIG_VIDEO_V4L2_SUBDEV_API
	sd->internal_ops = &ov569x_internal_ops;
	sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
#endif
#if defined(CONFIG_MEDIA_CONTROLLER)
	ov569x->pad.flags = MEDIA_PAD_FL_SOURCE;
	sd->entity.function = MEDIA_ENT_F_CAM_SENSOR;
	ret = media_entity_pads_init(&sd->entity, 1, &ov569x->pad);
	if (ret < 0)
		goto err_power_off;
#endif

	ret = v4l2_async_register_subdev_sensor_common(sd);
	if (ret) {
		dev_err(dev, "v4l2 async register subdev failed\n");
		goto err_clean_entity;
	}

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_idle(dev);

	return 0;

err_clean_entity:
#if defined(CONFIG_MEDIA_CONTROLLER)
	media_entity_cleanup(&sd->entity);
#endif
err_power_off:
	__ov569x_power_off(ov569x);
err_free_handler:
	v4l2_ctrl_handler_free(&ov569x->ctrl_handler);
err_destroy_mutex:
	mutex_destroy(&ov569x->mutex);

	/* For ACPI-based systems */
	if (ov569x->is_acpi_based)
		gpio_crs_put(ov569x);

	return ret;
}

static int ov569x_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct ov569x *ov569x = to_ov569x(sd);

	dev_info(&client->dev, "%s() called\n", __func__);

	/* For ACPI-based systems */
	if (ov569x->is_acpi_based)
		gpio_crs_put(ov569x);

	v4l2_async_unregister_subdev(sd);
#if defined(CONFIG_MEDIA_CONTROLLER)
	media_entity_cleanup(&sd->entity);
#endif
	v4l2_ctrl_handler_free(&ov569x->ctrl_handler);
	mutex_destroy(&ov569x->mutex);

	pm_runtime_disable(&client->dev);
	if (!pm_runtime_status_suspended(&client->dev))
		__ov569x_power_off(ov569x);
	pm_runtime_set_suspended(&client->dev);

	return 0;
}

#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id ov569x_of_match[] = {
	{ .compatible = "ovti,ov5695" },
	{},
};
MODULE_DEVICE_TABLE(of, ov569x_of_match);
#endif

#if IS_ENABLED(CONFIG_ACPI)
static const struct acpi_device_id ov569x_acpi_ids[] = {
	{"INT33BE"},
	{},
};
MODULE_DEVICE_TABLE(acpi, ov569x_acpi_ids);
#endif

static struct i2c_driver ov569x_i2c_driver = {
	.driver = {
		.name = "ov569x",
		.pm = &ov569x_pm_ops,
		.of_match_table = of_match_ptr(ov569x_of_match),
		.acpi_match_table = ACPI_PTR(ov569x_acpi_ids),
	},
	.probe_new	= &ov569x_probe,
	.remove		= &ov569x_remove,
};

module_i2c_driver(ov569x_i2c_driver);

MODULE_DESCRIPTION("OmniVision ov569x sensor driver");
MODULE_LICENSE("GPL v2");
