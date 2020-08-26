// SPDX-License-Identifier: GPL-2.0
/*
 * OV8865 MIPI Camera Subdev Driver
 * Copyright (C) 2020 Kévin L'hôpital.
 * Based on the ov5640 driver and an out of tree ov8865 driver by Allwinner.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/regulator/consumer.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-event.h>
#include <media/v4l2-fwnode.h>
#include <media/v4l2-subdev.h>

/* System */

#define OV8865_SW_STANDBY_REG		0x0100
#define OV8865_SW_STANDBY_STANDBY_N	BIT(0)

#define OV8865_SW_RESET_REG		0x0103

#define OV8865_PLL_CTRL2_REG		0x0302
#define OV8865_PLL_CTRL3_REG		0x0303
#define OV8865_PLL_CTRL4_REG		0x0304
#define OV8865_PLL_CTRLE_REG		0x030e
#define OV8865_PLL_CTRLF_REG		0x030f
#define OV8865_PLL_CTRL12_REG		0x0312
#define OV8865_PLL_CTRL1E_REG		0x031e

#define OV8865_SLAVE_ID_REG		0x3004
#define OV8865_SLAVE_ID_DEFAULT		0x36

#define OV8865_PUMP_CLK_DIV_REG		0x3015

#define OV8865_MIPI_CTRL_REG		0x3018
#define OV8865_CLOCK_SEL_REG		0x3020
#define OV8865_MIPI_SC_CTRL_REG		0X3022

#define OV8865_CHIP_ID_REG		0x300a
#define OV8865_CHIP_ID			0x008865

/* Exposure/gain/banding */

#define OV8865_EXPOSURE_CTRL_HH_REG	0x3500
#define OV8865_EXPOSURE_CTRL_H_REG	0x3501
#define OV8865_EXPOSURE_CTRL_L_REG	0x3502
#define OV8865_MANUAL_CTRL_REG		0x3503
#define OV8865_GAIN_CTRL_H_REG		0x3508
#define OV8865_GAIN_CTRL_L_REG		0x3509

#define OV8865_ASP_CTRL41_REG		0x3641
#define OV8865_ASP_CTRL46_REG		0x3646
#define OV8865_ASP_CTRL47_REG		0x3647
#define OV8865_ASP_CTRL50_REG		0x364a

/* Timing control */
#define OV8865_X_ADDR_START_H_REG	0x3800
#define OV8865_X_ADDR_START_L_REG	0x3801
#define OV8865_Y_ADDR_START_H_REG	0x3802
#define OV8865_Y_ADDR_START_L_REG	0x3803
#define OV8865_X_ADDR_END_H_REG		0x3804
#define OV8865_X_ADDR_END_L_REG		0x3805
#define OV8865_Y_ADDR_END_H_REG		0x3806
#define OV8865_Y_ADDR_END_L_REG		0x3807
#define OV8865_X_OUTPUT_SIZE_REG	0x3808
#define OV8865_Y_OUTPUT_SIZE_REG	0x380a
#define OV8865_HTS_REG			0x380c
#define OV8865_VTS_REG			0x380e
#define OV8865_ISP_X_WIN_H_REG		0x3810
#define OV8865_ISP_X_WIN_L_REG		0x3811
#define OV8865_ISP_Y_WIN_L_REG		0x3813
#define OV8865_X_INC_ODD_REG		0x3814
#define OV8865_X_INC_EVEN_REG		0x3815
#define OV8865_FORMAT1_REG		0x3820
#define OV8865_FORMAT1_MIRROR_ARR	BIT(1)
#define OV8865_FORMAT1_MIRROR_DIG	BIT(2)
#define OV8865_FORMAT2_REG		0x3821
#define OV8865_FORMAT2_MIRROR_ARR	BIT(1)
#define OV8865_FORMAT2_MIRROR_DIG	BIT(2)
#define OV8865_Y_INC_ODD_REG		0x382a
#define OV8865_Y_INC_EVEN_REG		0x382b
#define OV8865_BLC_NUM_OPTION_REG	0x3830
#define OV8865_ZLINE_NUM_OPTION_REG	0x3836
#define OV8865_RGBC_REG			0x3837
#define OV8865_AUTO_SIZE_CTRL0_REG	0x3841
#define OV8865_BOUNDARY_PIX_NUM_REG	0x3846

/* OTP */

#define OV8865_OTP_REG			0x3d85
#define OV8865_OTP_SETT_STT_ADDR_H_REG	0x3d8c
#define OV8865_OTP_SETT_STT_ADDR_L_REG	0x3d8d

/* Black Level */

#define OV8865_BLC_CTRL0_REG		0x4000
#define OV8865_BLC_CTRL1_REG		0x4001
#define OV8865_BLC_CTRL5_REG		0x4005
#define OV8865_BLC_CTRLB_REG            0x400b
#define OV8865_BLC_CTRLD_REG            0x400d
#define OV8865_BLC_CTRL1B_REG           0x401b
#define OV8865_BLC_CTRL1D_REG           0x401d
#define OV8865_BLC_CTRL1F_REG		0x401f
#define OV8865_ANCHOR_LEFT_START_H_REG	0x4020
#define OV8865_ANCHOR_LEFT_START_L_REG	0x4021
#define OV8865_ANCHOR_LEFT_END_H_REG	0x4022
#define OV8865_ANCHOR_LEFT_END_L_REG	0x4023
#define OV8865_ANCHOR_RIGHT_START_H_REG	0x4024
#define OV8865_ANCHOR_RIGHT_START_L_REG	0x4025
#define OV8865_ANCHOR_RIGHT_END_H_REG	0x4026
#define OV8865_ANCHOR_RIGHT_END_L_REG	0x4027
#define OV8865_TOP_ZLINE_ST_REG		0x4028
#define OV8865_TOP_ZLINE_NUM_REG	0x4029
#define OV8865_TOP_BKLINE_ST_REG	0x402a
#define OV8865_TOP_BKLINE_NUM_REG	0x402b
#define OV8865_BOT_ZLINE_ST_REG		0x402c
#define OV8865_BOT_ZLINE_NUM_REG	0x402d
#define OV8865_BOT_BLKLINE_ST_REG	0x402e
#define OV8865_BOT_BLKLINE_NUM_REG	0x402f
#define OV8865_BLC_OFFSET_LIMIT_REG	0x4034

/* Format Control */

#define OV8865_CLIP_MAX_HI_REG		0x4300
#define OV8865_CLIP_MIN_HI_REG		0x4301
#define OV8865_CLIP_LO_REG		0x4302

#define OV8865_R_VFIFO_READ_START_REG	0x4601

/* MIPI Control */

#define OV8865_MIPI_CTRL13_REG		0x4813
#define OV8865_CLK_PREPARE_MIN_REG	0x481f
#define OV8865_PCLK_PERIOD_REG		0x4837
#define OV8865_LANE_SEL01_REG		0x4850
#define OV8865_LANE_SEL23_REG		0x4851

/* LVDS Control */

#define OV8865_LVDS_R0_REG		0x4b00
#define OV8865_LVDS_BLK_TIMES_H_REG	0x4b0c
#define OV8865_LVDS_BLK_TIMES_L_REG	0x4b0d

/* DSP Control */

#define OV8865_ISP_CTRL0_REG		0x5000
#define OV8865_ISP_CTRL1_REG		0x5001
#define OV8865_ISP_CTRL2_REG		0x5002

#define OV8865_AVG_READOUT_REG		0x568a

/* Pre DSP Control */

#define OV8865_PRE_CTRL0		0x5e00
#define OV8865_PRE_CTRL1		0x5e01

/* OTP DPC Control */

#define OV8865_OTP_CTRL0		0x5b00
#define OV8865_OTP_CTRL1		0x5b01
#define OV8865_OTP_CTRL2		0x5b02
#define OV8865_OTP_CTRL3		0x5b03
#define OV8865_OTP_CTRL5		0x5b05

/* LENC Control */

#define OV8865_LENC_G0_REG		0x5800
#define OV8865_LENC_G1_REG		0x5801
#define OV8865_LENC_G2_REG		0x5802
#define OV8865_LENC_G3_REG		0x5803
#define OV8865_LENC_G4_REG		0x5804
#define OV8865_LENC_G5_REG		0x5805
#define OV8865_LENC_G10_REG		0x5806
#define OV8865_LENC_G11_REG		0x5807
#define OV8865_LENC_G12_REG		0x5808
#define OV8865_LENC_G13_REG		0x5809
#define OV8865_LENC_G14_REG		0x580a
#define OV8865_LENC_G15_REG		0x580b
#define OV8865_LENC_G20_REG		0x580c
#define OV8865_LENC_G21_REG		0x580d
#define OV8865_LENC_G22_REG		0x580e
#define OV8865_LENC_G23_REG		0x580f
#define OV8865_LENC_G24_REG		0x5810
#define OV8865_LENC_G25_REG		0x5811
#define OV8865_LENC_G30_REG		0x5812
#define OV8865_LENC_G31_REG		0x5813
#define OV8865_LENC_G32_REG		0x5814
#define OV8865_LENC_G33_REG		0x5815
#define OV8865_LENC_G34_REG		0x5816
#define OV8865_LENC_G35_REG		0x5817
#define OV8865_LENC_G40_REG		0x5818
#define OV8865_LENC_G41_REG		0x5819
#define OV8865_LENC_G42_REG		0x581a
#define OV8865_LENC_G43_REG		0x581b
#define OV8865_LENC_G44_REG		0x581c
#define OV8865_LENC_G45_REG		0x581d
#define OV8865_LENC_G50_REG		0x581e
#define OV8865_LENC_G51_REG		0x581f
#define OV8865_LENC_G52_REG		0x5820
#define OV8865_LENC_G53_REG		0x5821
#define OV8865_LENC_G54_REG		0x5822
#define OV8865_LENC_G55_REG		0x5823
#define OV8865_LENC_BR0_REG		0x5824
#define OV8865_LENC_BR1_REG		0x5825
#define OV8865_LENC_BR2_REG		0x5826
#define OV8865_LENC_BR3_REG		0x5827
#define OV8865_LENC_BR4_REG		0x5828
#define OV8865_LENC_BR10_REG		0x5829
#define OV8865_LENC_BR11_REG		0x582a
#define OV8865_LENC_BR12_REG		0x582b
#define OV8865_LENC_BR13_REG		0x582c
#define OV8865_LENC_BR14_REG		0x582d
#define OV8865_LENC_BR20_REG		0x582e
#define OV8865_LENC_BR21_REG		0x582f
#define OV8865_LENC_BR22_REG		0x5830
#define OV8865_LENC_BR23_REG		0x5831
#define OV8865_LENC_BR24_REG		0x5832
#define OV8865_LENC_BR30_REG		0x5833
#define OV8865_LENC_BR31_REG		0x5834
#define OV8865_LENC_BR32_REG		0x5835
#define OV8865_LENC_BR33_REG		0x5836
#define OV8865_LENC_BR34_REG		0x5837
#define OV8865_LENC_BR40_REG		0x5838
#define OV8865_LENC_BR41_REG		0x5839
#define OV8865_LENC_BR42_REG		0x583a
#define OV8865_LENC_BR43_REG		0x583b
#define OV8865_LENC_BR44_REG		0x583c
#define OV8865_LENC_BROFFSET_REG	0x583d

enum ov8865_mode_id {
	OV8865_MODE_QUXGA_3264_2448 = 0,
	OV8865_MODE_6M_3264_1836,
	OV8865_MODE_1080P_1920_1080,
	OV8865_MODE_720P_1280_720,
	OV8865_MODE_UXGA_1600_1200,
	OV8865_MODE_SVGA_800_600,
	OV8865_MODE_VGA_640_480,
	OV8865_NUM_MODES,
};


enum ov8865_frame_rate {
	OV8865_30_FPS = 0,
	OV8865_90_FPS,
	OV8865_NUM_FRAMERATES,
};

static const int ov8865_framerates[] = {
	[OV8865_30_FPS] = 30,
	[OV8865_90_FPS] = 90,
};

struct ov8865_pixfmt {
	u32 code;
	u32 colorspace;
};

static const struct ov8865_pixfmt ov8865_formats[] = {
	{ MEDIA_BUS_FMT_SBGGR10_1X10, V4L2_COLORSPACE_RAW, },
};

/* regulator supplies */
static const char * const ov8865_supply_names[] = {
	"AVDD",  /* Analog (2.8V) supply */
	"DOVDD", /* Digital I/O (1,8V/2.8V) supply */
	"VDD2",  /* Digital Core (1.2V) supply */
	"AFVDD",
};

#define OV8865_NUM_SUPPLIES ARRAY_SIZE(ov8865_supply_names)

struct reg_value {
	u16 reg_addr;
	u8 val;
	u32 delay_ms;
};

struct ov8865_mode_info {
	enum ov8865_mode_id id;
	u32 hact;
	u32 htot;
	u32 vact;
	u32 vtot;
	const struct reg_value *reg_data;
	u32 reg_data_size;
};

struct ov8865_ctrls {
	struct v4l2_ctrl_handler handler;
	struct v4l2_ctrl *pixel_rate;
	struct v4l2_ctrl *exposure;
	struct v4l2_ctrl *gain;
	struct v4l2_ctrl *hflip;
	struct v4l2_ctrl *vflip;
};

struct ov8865_dev {
	struct i2c_client *i2c_client;
	struct v4l2_subdev sd;
	struct media_pad pad;
	struct v4l2_fwnode_endpoint ep;
	struct clk *xclk;
	u32 xclk_freq;

	struct regulator_bulk_data supplies[OV8865_NUM_SUPPLIES];
	struct gpio_desc *reset_gpio;
	struct gpio_desc *pwdn_gpio;
	bool upside_down;

	struct mutex lock;

	int power_count;

	struct v4l2_mbus_framefmt fmt;

	const struct ov8865_mode_info *current_mode;
	const struct ov8865_mode_info *last_mode;
	enum ov8865_frame_rate current_fr;
	struct v4l2_fract frame_interval;
	struct ov8865_ctrls ctrls;

	bool streaming;
};

static inline struct ov8865_dev *to_ov8865_dev(struct v4l2_subdev *sd)
{
	return container_of(sd, struct ov8865_dev, sd);
}

static inline struct v4l2_subdev *ctrl_to_sd(struct v4l2_ctrl *ctrl)
{
	return &container_of(ctrl->handler, struct ov8865_dev,
			     ctrls.handler)->sd;
}

static const struct reg_value ov8865_init_setting_QUXGA[] = {
	{ OV8865_SW_RESET_REG, 0x01, 16 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
	{ 0x3638, 0xff },
	{ OV8865_PUMP_CLK_DIV_REG, 0x01 },
	{ OV8865_MIPI_SC_CTRL_REG, 0x01 },
	{ 0x3031, 0x0a },
	{ 0x3305, 0xf1 },
	{ 0x3308, 0x00 },
	{ 0x3309, 0x28 },
	{ 0x330a, 0x00 },
	{ 0x330b, 0x20 },
	{ 0x330c, 0x00 },
	{ 0x330d, 0x00 },
	{ 0x330e, 0x00 },
	{ 0x330f, 0x40 },
	{ 0x3307, 0x04 },
	{ 0x3604, 0x04 },
	{ 0x3602, 0x30 },
	{ 0x3605, 0x00 },
	{ 0x3607, 0x20 },
	{ 0x3608, 0x11 },
	{ 0x3609, 0x68 },
	{ 0x360a, 0x40 },
	{ 0x360c, 0xdd },
	{ 0x360e, 0x0c },
	{ 0x3610, 0x07 },
	{ 0x3612, 0x86 },
	{ 0x3613, 0x58 },
	{ 0x3614, 0x28 },
	{ 0x3617, 0x40 },
	{ 0x3618, 0x5a },
	{ 0x3619, 0x9b },
	{ 0x361c, 0x00 },
	{ 0x361d, 0x60 },
	{ 0x3631, 0x60 },
	{ 0x3633, 0x10 },
	{ 0x3634, 0x10 },
	{ 0x3635, 0x10 },
	{ 0x3636, 0x10 },
	{ OV8865_ASP_CTRL41_REG, 0x55 },
	{ OV8865_ASP_CTRL46_REG, 0x86 },
	{ OV8865_ASP_CTRL47_REG, 0x27 },
	{ OV8865_ASP_CTRL50_REG, 0x1b },
	{ OV8865_EXPOSURE_CTRL_HH_REG, 0x00 },
	{ OV8865_EXPOSURE_CTRL_H_REG, 0x4c },
	{ OV8865_EXPOSURE_CTRL_L_REG, 0x00 },
	{ OV8865_MANUAL_CTRL_REG, 0x00 },
	{ OV8865_GAIN_CTRL_H_REG, 0x02 },
	{ OV8865_GAIN_CTRL_L_REG, 0x00 },
	{ 0x3700, 0x24 },
	{ 0x3701, 0x0c },
	{ 0x3702, 0x28 },
	{ 0x3703, 0x19 },
	{ 0x3704, 0x14 },
	{ 0x3705, 0x00 },
	{ 0x3706, 0x38 },
	{ 0x3707, 0x04 },
	{ 0x3708, 0x24 },
	{ 0x3709, 0x40 },
	{ 0x370a, 0x00 },
	{ 0x370b, 0xb8 },
	{ 0x370c, 0x04 },
	{ 0x3718, 0x12 },
	{ 0x3719, 0x31 },
	{ 0x3712, 0x42 },
	{ 0x3714, 0x12 },
	{ 0x371e, 0x19 },
	{ 0x371f, 0x40 },
	{ 0x3720, 0x05 },
	{ 0x3721, 0x05 },
	{ 0x3724, 0x02 },
	{ 0x3725, 0x02 },
	{ 0x3726, 0x06 },
	{ 0x3728, 0x05 },
	{ 0x3729, 0x02 },
	{ 0x372a, 0x03 },
	{ 0x372b, 0x53 },
	{ 0x372c, 0xa3 },
	{ 0x372d, 0x53 },
	{ 0x372e, 0x06 },
	{ 0x372f, 0x10 },
	{ 0x3730, 0x01 },
	{ 0x3731, 0x06 },
	{ 0x3732, 0x14 },
	{ 0x3733, 0x10 },
	{ 0x3734, 0x40 },
	{ 0x3736, 0x20 },
	{ 0x373a, 0x02 },
	{ 0x373b, 0x0c },
	{ 0x373c, 0x0a },
	{ 0x373e, 0x03 },
	{ 0x3755, 0x40 },
	{ 0x3758, 0x00 },
	{ 0x3759, 0x4c },
	{ 0x375a, 0x06 },
	{ 0x375b, 0x13 },
	{ 0x375c, 0x40 },
	{ 0x375d, 0x02 },
	{ 0x375e, 0x00 },
	{ 0x375f, 0x14 },
	{ 0x3767, 0x1c },
	{ 0x3768, 0x04 },
	{ 0x3769, 0x20 },
	{ 0x376c, 0xc0 },
	{ 0x376d, 0xc0 },
	{ 0x376a, 0x08 },
	{ 0x3761, 0x00 },
	{ 0x3762, 0x00 },
	{ 0x3763, 0x00 },
	{ 0x3766, 0xff },
	{ 0x376b, 0x42 },
	{ 0x3772, 0x23 },
	{ 0x3773, 0x02 },
	{ 0x3774, 0x16 },
	{ 0x3775, 0x12 },
	{ 0x3776, 0x08 },
	{ 0x37a0, 0x44 },
	{ 0x37a1, 0x3d },
	{ 0x37a2, 0x3d },
	{ 0x37a3, 0x01 },
	{ 0x37a4, 0x00 },
	{ 0x37a5, 0x08 },
	{ 0x37a6, 0x00 },
	{ 0x37a7, 0x44 },
	{ 0x37a8, 0x58 },
	{ 0x37a9, 0x58 },
	{ 0x3760, 0x00 },
	{ 0x376f, 0x01 },
	{ 0x37aa, 0x44 },
	{ 0x37ab, 0x2e },
	{ 0x37ac, 0x2e },
	{ 0x37ad, 0x33 },
	{ 0x37ae, 0x0d },
	{ 0x37af, 0x0d },
	{ 0x37b0, 0x00 },
	{ 0x37b1, 0x00 },
	{ 0x37b2, 0x00 },
	{ 0x37b3, 0x42 },
	{ 0x37b4, 0x42 },
	{ 0x37b5, 0x33 },
	{ 0x37b6, 0x00 },
	{ 0x37b7, 0x00 },
	{ 0x37b8, 0x00 },
	{ 0x37b9, 0xff },
	{ OV8865_OTP_REG, 0x06 },
	{ OV8865_OTP_SETT_STT_ADDR_H_REG, 0x75 },
	{ OV8865_OTP_SETT_STT_ADDR_L_REG, 0xef },
	{ 0x3f08, 0x0b },
	{ OV8865_CLIP_MAX_HI_REG, 0xff },
	{ OV8865_CLIP_MIN_HI_REG, 0x00 },
	{ OV8865_CLIP_LO_REG, 0x0f },
	{ 0x4500, 0x40 },
	{ 0x4503, 0x10 },
	{ OV8865_R_VFIFO_READ_START_REG, 0x74 },
	{ OV8865_CLK_PREPARE_MIN_REG, 0x32 },
	{ OV8865_PCLK_PERIOD_REG, 0x16 },
	{ OV8865_LANE_SEL01_REG, 0x10 },
	{ OV8865_LANE_SEL23_REG, 0x32 },
	{ OV8865_LVDS_R0_REG, 0x2a },
	{ OV8865_LVDS_BLK_TIMES_L_REG, 0x00 },
	{ 0x4d00, 0x04 },
	{ 0x4d01, 0x18 },
	{ 0x4d02, 0xc3 },
	{ 0x4d03, 0xff },
	{ 0x4d04, 0xff },
	{ 0x4d05, 0xff },
	{ OV8865_ISP_CTRL0_REG, 0x96 },
	{ OV8865_ISP_CTRL1_REG, 0x01 },
	{ OV8865_ISP_CTRL2_REG, 0x08 },
	{ 0x5901, 0x00 },
	{ OV8865_PRE_CTRL0, 0x00 },
	{ OV8865_PRE_CTRL1, 0x41 },
	{ OV8865_SW_STANDBY_REG, OV8865_SW_STANDBY_STANDBY_N },
	{ OV8865_OTP_CTRL0, 0x02 },
	{ OV8865_OTP_CTRL1, 0xd0 },
	{ OV8865_OTP_CTRL2, 0x03 },
	{ OV8865_OTP_CTRL3, 0xff },
	{ OV8865_OTP_CTRL5, 0x6c },
	{ 0x5780, 0xfc },
	{ 0x5781, 0xdf },
	{ 0x5782, 0x3f },
	{ 0x5783, 0x08 },
	{ 0x5784, 0x0c },
	{ 0x5786, 0x20 },
	{ 0x5787, 0x40 },
	{ 0x5788, 0x08 },
	{ 0x5789, 0x08 },
	{ 0x578a, 0x02 },
	{ 0x578b, 0x01 },
	{ 0x578c, 0x01 },
	{ 0x578d, 0x0c },
	{ 0x578e, 0x02 },
	{ 0x578f, 0x01 },
	{ 0x5790, 0x01 },
	{ OV8865_LENC_G0_REG, 0x1d },
	{ OV8865_LENC_G1_REG, 0x0e },
	{ OV8865_LENC_G2_REG, 0x0c },
	{ OV8865_LENC_G3_REG, 0x0c },
	{ OV8865_LENC_G4_REG, 0x0f },
	{ OV8865_LENC_G5_REG, 0x22 },
	{ OV8865_LENC_G10_REG, 0x0a },
	{ OV8865_LENC_G11_REG, 0x06 },
	{ OV8865_LENC_G12_REG, 0x05 },
	{ OV8865_LENC_G13_REG, 0x05 },
	{ OV8865_LENC_G14_REG, 0x07 },
	{ OV8865_LENC_G15_REG, 0x0a },
	{ OV8865_LENC_G20_REG, 0x06 },
	{ OV8865_LENC_G21_REG, 0x02 },
	{ OV8865_LENC_G22_REG, 0x00 },
	{ OV8865_LENC_G23_REG, 0x00 },
	{ OV8865_LENC_G24_REG, 0x03 },
	{ OV8865_LENC_G25_REG, 0x07 },
	{ OV8865_LENC_G30_REG, 0x06 },
	{ OV8865_LENC_G31_REG, 0x02 },
	{ OV8865_LENC_G32_REG, 0x00 },
	{ OV8865_LENC_G33_REG, 0x00 },
	{ OV8865_LENC_G34_REG, 0x03 },
	{ OV8865_LENC_G35_REG, 0x07 },
	{ OV8865_LENC_G40_REG, 0x09 },
	{ OV8865_LENC_G41_REG, 0x06 },
	{ OV8865_LENC_G42_REG, 0x04 },
	{ OV8865_LENC_G43_REG, 0x04 },
	{ OV8865_LENC_G44_REG, 0x06 },
	{ OV8865_LENC_G45_REG, 0x0a },
	{ OV8865_LENC_G50_REG, 0x19 },
	{ OV8865_LENC_G51_REG, 0x0d },
	{ OV8865_LENC_G52_REG, 0x0b },
	{ OV8865_LENC_G53_REG, 0x0b },
	{ OV8865_LENC_G54_REG, 0x0e },
	{ OV8865_LENC_G55_REG, 0x22 },
	{ OV8865_LENC_BR0_REG, 0x23 },
	{ OV8865_LENC_BR1_REG, 0x28 },
	{ OV8865_LENC_BR2_REG, 0x29 },
	{ OV8865_LENC_BR3_REG, 0x27 },
	{ OV8865_LENC_BR4_REG, 0x13 },
	{ OV8865_LENC_BR10_REG, 0x26 },
	{ OV8865_LENC_BR11_REG, 0x33 },
	{ OV8865_LENC_BR12_REG, 0x32 },
	{ OV8865_LENC_BR13_REG, 0x33 },
	{ OV8865_LENC_BR14_REG, 0x16 },
	{ OV8865_LENC_BR20_REG, 0x14 },
	{ OV8865_LENC_BR21_REG, 0x30 },
	{ OV8865_LENC_BR22_REG, 0x31 },
	{ OV8865_LENC_BR23_REG, 0x30 },
	{ OV8865_LENC_BR24_REG, 0x15 },
	{ OV8865_LENC_BR30_REG, 0x26 },
	{ OV8865_LENC_BR31_REG, 0x23 },
	{ OV8865_LENC_BR32_REG, 0x21 },
	{ OV8865_LENC_BR33_REG, 0x23 },
	{ OV8865_LENC_BR34_REG, 0x05 },
	{ OV8865_LENC_BR40_REG, 0x36 },
	{ OV8865_LENC_BR41_REG, 0x27 },
	{ OV8865_LENC_BR42_REG, 0x28 },
	{ OV8865_LENC_BR43_REG, 0x26 },
	{ OV8865_LENC_BR44_REG, 0x24 },
	{ OV8865_LENC_BROFFSET_REG, 0xdf },
	{ OV8865_SW_STANDBY_REG, 0x00 },
};

static const struct reg_value ov8865_setting_QUXGA[] = {
	{ OV8865_SW_STANDBY_REG, 0x00, 5 },
	{ 0x3501, 0x98 },
	{ 0x3502, 0x60 },
	{ 0x3700, 0x48 },
	{ 0x3701, 0x18 },
	{ 0x3702, 0x50 },
	{ 0x3703, 0x32 },
	{ 0x3704, 0x28 },
	{ 0x3706, 0x70 },
	{ 0x3707, 0x08 },
	{ 0x3708, 0x48 },
	{ 0x3709, 0x80 },
	{ 0x370a, 0x01 },
	{ 0x370b, 0x70 },
	{ 0x370c, 0x07 },
	{ 0x3718, 0x14 },
	{ 0x3712, 0x44 },
	{ 0x371e, 0x31 },
	{ 0x371f, 0x7f },
	{ 0x3720, 0x0a },
	{ 0x3721, 0x0a },
	{ 0x3724, 0x04 },
	{ 0x3725, 0x04 },
	{ 0x3726, 0x0c },
	{ 0x3728, 0x0a },
	{ 0x3729, 0x03 },
	{ 0x372a, 0x06 },
	{ 0x372b, 0xa6 },
	{ 0x372c, 0xa6 },
	{ 0x372d, 0xa6 },
	{ 0x372e, 0x0c },
	{ 0x372f, 0x20 },
	{ 0x3730, 0x02 },
	{ 0x3731, 0x0c },
	{ 0x3732, 0x28 },
	{ 0x3736, 0x30 },
	{ 0x373a, 0x04 },
	{ 0x373b, 0x18 },
	{ 0x373c, 0x14 },
	{ 0x373e, 0x06 },
	{ 0x375a, 0x0c },
	{ 0x375b, 0x26 },
	{ 0x375d, 0x04 },
	{ 0x375f, 0x28 },
	{ 0x3767, 0x1e },
	{ 0x3772, 0x46 },
	{ 0x3773, 0x04 },
	{ 0x3774, 0x2c },
	{ 0x3775, 0x13 },
	{ 0x3776, 0x10 },
	{ 0x37a0, 0x88 },
	{ 0x37a1, 0x7a },
	{ 0x37a2, 0x7a },
	{ 0x37a3, 0x02 },
	{ 0x37a5, 0x09 },
	{ 0x37a7, 0x88 },
	{ 0x37a8, 0xb0 },
	{ 0x37a9, 0xb0 },
	{ 0x37aa, 0x88 },
	{ 0x37ab, 0x5c },
	{ 0x37ac, 0x5c },
	{ 0x37ad, 0x55 },
	{ 0x37ae, 0x19 },
	{ 0x37af, 0x19 },
	{ 0x37b3, 0x84 },
	{ 0x37b4, 0x84 },
	{ 0x37b5, 0x66 },
	{ 0x3f08, 0x16 },
	{ 0x4500, 0x68 },
	{ OV8865_R_VFIFO_READ_START_REG, 0x10 },
	{ OV8865_ISP_CTRL2_REG, 0x08 },
	{ 0x5901, 0x00 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
};

static const struct reg_value ov8865_setting_6M[] = {
	{ OV8865_SW_STANDBY_REG, 0x00, 5 },
	{ 0x3501, 0x72 },
	{ 0x3502, 0x20 },
	{ 0x3700, 0x48 },
	{ 0x3701, 0x18 },
	{ 0x3702, 0x50 },
	{ 0x3703, 0x32 },
	{ 0x3704, 0x28 },
	{ 0x3706, 0x70 },
	{ 0x3707, 0x08 },
	{ 0x3708, 0x48 },
	{ 0x3709, 0x80 },
	{ 0x370a, 0x01 },
	{ 0x370b, 0x70 },
	{ 0x370c, 0x07 },
	{ 0x3718, 0x14 },
	{ 0x3712, 0x44 },
	{ 0x371e, 0x31 },
	{ 0x371f, 0x7f },
	{ 0x3720, 0x0a },
	{ 0x3721, 0x0a },
	{ 0x3724, 0x04 },
	{ 0x3725, 0x04 },
	{ 0x3726, 0x0c },
	{ 0x3728, 0x0a },
	{ 0x3729, 0x03 },
	{ 0x372a, 0x06 },
	{ 0x372b, 0xa6 },
	{ 0x372c, 0xa6 },
	{ 0x372d, 0xa6 },
	{ 0x372e, 0x0c },
	{ 0x372f, 0x20 },
	{ 0x3730, 0x02 },
	{ 0x3731, 0x0c },
	{ 0x3732, 0x28 },
	{ 0x3736, 0x30 },
	{ 0x373a, 0x04 },
	{ 0x373b, 0x18 },
	{ 0x373c, 0x14 },
	{ 0x373e, 0x06 },
	{ 0x375a, 0x0c },
	{ 0x375b, 0x26 },
	{ 0x375d, 0x04 },
	{ 0x375f, 0x28 },
	{ 0x3767, 0x1e },
	{ 0x3772, 0x46 },
	{ 0x3773, 0x04 },
	{ 0x3774, 0x2c },
	{ 0x3775, 0x13 },
	{ 0x3776, 0x10 },
	{ 0x37a0, 0x88 },
	{ 0x37a1, 0x7a },
	{ 0x37a2, 0x7a },
	{ 0x37a3, 0x02 },
	{ 0x37a5, 0x09 },
	{ 0x37a7, 0x88 },
	{ 0x37a8, 0xb0 },
	{ 0x37a9, 0xb0 },
	{ 0x37aa, 0x88 },
	{ 0x37ab, 0x5c },
	{ 0x37ac, 0x5c },
	{ 0x37ad, 0x55 },
	{ 0x37ae, 0x19 },
	{ 0x37af, 0x19 },
	{ 0x37b3, 0x84 },
	{ 0x37b4, 0x84 },
	{ 0x37b5, 0x66 },
	{ 0x3f08, 0x16 },
	{ 0x4500, 0x68 },
	{ OV8865_R_VFIFO_READ_START_REG, 0x10 },
	{ OV8865_ISP_CTRL2_REG, 0x08 },
	{ 0x5901, 0x00 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
};


static const struct reg_value ov8865_setting_UXGA[] = {
	{ OV8865_SW_STANDBY_REG, 0x00, 5 },
	{ 0x3501, 0x4c },
	{ 0x3502, 0x00 },
	{ 0x3700, 0x24 },
	{ 0x3701, 0x0c },
	{ 0x3702, 0x28 },
	{ 0x3703, 0x19 },
	{ 0x3704, 0x14 },
	{ 0x3706, 0x38 },
	{ 0x3707, 0x04 },
	{ 0x3708, 0x24 },
	{ 0x3709, 0x40 },
	{ 0x370a, 0x00 },
	{ 0x370b, 0xb8 },
	{ 0x370c, 0x04 },
	{ 0x3718, 0x12 },
	{ 0x3712, 0x42 },
	{ 0x371e, 0x19 },
	{ 0x371f, 0x40 },
	{ 0x3720, 0x05 },
	{ 0x3721, 0x05 },
	{ 0x3724, 0x02 },
	{ 0x3725, 0x02 },
	{ 0x3726, 0x06 },
	{ 0x3728, 0x05 },
	{ 0x3729, 0x02 },
	{ 0x372a, 0x03 },
	{ 0x372b, 0x53 },
	{ 0x372c, 0xa3 },
	{ 0x372d, 0x53 },
	{ 0x372e, 0x06 },
	{ 0x372f, 0x10 },
	{ 0x3730, 0x01 },
	{ 0x3731, 0x06 },
	{ 0x3732, 0x14 },
	{ 0x3736, 0x20 },
	{ 0x373a, 0x02 },
	{ 0x373b, 0x0c },
	{ 0x373c, 0x0a },
	{ 0x373e, 0x03 },
	{ 0x375a, 0x06 },
	{ 0x375b, 0x13 },
	{ 0x375d, 0x02 },
	{ 0x375f, 0x14 },
	{ 0x3767, 0x1c },
	{ 0x3772, 0x23 },
	{ 0x3773, 0x02 },
	{ 0x3774, 0x16 },
	{ 0x3775, 0x12 },
	{ 0x3776, 0x08 },
	{ 0x37a0, 0x44 },
	{ 0x37a1, 0x3d },
	{ 0x37a2, 0x3d },
	{ 0x37a3, 0x01 },
	{ 0x37a5, 0x08 },
	{ 0x37a7, 0x44 },
	{ 0x37a8, 0x58 },
	{ 0x37a9, 0x58 },
	{ 0x37aa, 0x44 },
	{ 0x37ab, 0x2e },
	{ 0x37ac, 0x2e },
	{ 0x37ad, 0x33 },
	{ 0x37ae, 0x0d },
	{ 0x37af, 0x0d },
	{ 0x37b3, 0x42 },
	{ 0x37b4, 0x42 },
	{ 0x37b5, 0x33 },
	{ 0x3f08, 0x0b },
	{ 0x4500, 0x40 },
	{ OV8865_R_VFIFO_READ_START_REG, 0x74 },
	{ OV8865_ISP_CTRL2_REG, 0x08 },
	{ 0x5901, 0x00 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
};

static const struct reg_value ov8865_setting_SVGA[] = {
	{ OV8865_SW_STANDBY_REG, 0x00, 5 },
	{ 0x3501, 0x26 },
	{ 0x3502, 0x00 },
	{ 0x3700, 0x24 },
	{ 0x3701, 0x0c },
	{ 0x3702, 0x28 },
	{ 0x3703, 0x19 },
	{ 0x3704, 0x14 },
	{ 0x3706, 0x38 },
	{ 0x3707, 0x04 },
	{ 0x3708, 0x24 },
	{ 0x3709, 0x40 },
	{ 0x370a, 0x00 },
	{ 0x370b, 0xb8 },
	{ 0x370c, 0x04 },
	{ 0x3718, 0x12 },
	{ 0x3712, 0x42 },
	{ 0x371e, 0x19 },
	{ 0x371f, 0x40 },
	{ 0x3720, 0x05 },
	{ 0x3721, 0x05 },
	{ 0x3724, 0x02 },
	{ 0x3725, 0x02 },
	{ 0x3726, 0x06 },
	{ 0x3728, 0x05 },
	{ 0x3729, 0x02 },
	{ 0x372a, 0x03 },
	{ 0x372b, 0x53 },
	{ 0x372c, 0xa3 },
	{ 0x372d, 0x53 },
	{ 0x372e, 0x06 },
	{ 0x372f, 0x10 },
	{ 0x3730, 0x01 },
	{ 0x3731, 0x06 },
	{ 0x3732, 0x14 },
	{ 0x3736, 0x20 },
	{ 0x373a, 0x02 },
	{ 0x373b, 0x0c },
	{ 0x373c, 0x0a },
	{ 0x373e, 0x03 },
	{ 0x375a, 0x06 },
	{ 0x375b, 0x13 },
	{ 0x375d, 0x02 },
	{ 0x375f, 0x14 },
	{ 0x3767, 0x18 },
	{ 0x3772, 0x23 },
	{ 0x3773, 0x02 },
	{ 0x3774, 0x16 },
	{ 0x3775, 0x12 },
	{ 0x3776, 0x08 },
	{ 0x37a0, 0x44 },
	{ 0x37a1, 0x3d },
	{ 0x37a2, 0x3d },
	{ 0x37a3, 0x01 },
	{ 0x37a5, 0x08 },
	{ 0x37a7, 0x44 },
	{ 0x37a8, 0x58 },
	{ 0x37a9, 0x58 },
	{ 0x37aa, 0x44 },
	{ 0x37ab, 0x2e },
	{ 0x37ac, 0x2e },
	{ 0x37ad, 0x33 },
	{ 0x37ae, 0x0d },
	{ 0x37af, 0x0d },
	{ 0x37b3, 0x42 },
	{ 0x37b4, 0x42 },
	{ 0x37b5, 0x33 },
	{ 0x3f08, 0x0b },
	{ 0x4500, 0x40 },
	{ OV8865_R_VFIFO_READ_START_REG, 0x50 },
	{ OV8865_ISP_CTRL2_REG, 0x0c },
	{ 0x5901, 0x04 },
	{ OV8865_SW_STANDBY_REG, 0x00 },
};

static const struct ov8865_mode_info ov8865_mode_init_data = {
	.id = 0,
	.hact = 3264,
	.htot = 1944,
	.vact = 2448,
	.vtot = 2470,
	.reg_data = ov8865_init_setting_QUXGA,
	.reg_data_size = ARRAY_SIZE(ov8865_init_setting_QUXGA),
};

static const struct ov8865_mode_info ov8865_mode_data[OV8865_NUM_MODES] = {
	{
		.id = OV8865_MODE_QUXGA_3264_2448,
		.hact = 3264,
		.htot = 1944,
		.vact = 2448,
		.vtot = 2470,
		.reg_data = ov8865_setting_QUXGA,
		.reg_data_size = ARRAY_SIZE(ov8865_setting_QUXGA)
	},
	{
		.id = OV8865_MODE_6M_3264_1836,
		.hact = 3264,
		.htot = 2582,
		.vact = 1836,
		.vtot = 1858,
		.reg_data = ov8865_setting_6M,
		.reg_data_size = ARRAY_SIZE(ov8865_setting_6M)
	},
	{
		.id = OV8865_MODE_1080P_1920_1080,
		.hact = 1920,
		.htot = 2582,
		.vact = 1080,
		.vtot = 1858,
		.reg_data = ov8865_setting_6M,
		.reg_data_size = ARRAY_SIZE(ov8865_setting_6M)
	},
	{
		.id = OV8865_MODE_720P_1280_720,
		.hact = 1280,
		.htot = 1923,
		.vact = 720,
		.vtot = 1248,
		.reg_data = ov8865_setting_UXGA,
		.reg_data_size = ARRAY_SIZE(ov8865_setting_UXGA)
	},
	{
		.id = OV8865_MODE_UXGA_1600_1200,
		.hact = 1600,
		.htot = 1923,
		.vact = 1200,
		.vtot = 1248,
		.reg_data = ov8865_setting_UXGA,
		.reg_data_size = ARRAY_SIZE(ov8865_setting_UXGA)
	},
	{
		.id = OV8865_MODE_SVGA_800_600,
		.hact = 800,
		.htot = 1250,
		.vact = 600,
		.vtot = 640,
		.reg_data = ov8865_setting_SVGA,
		.reg_data_size = ARRAY_SIZE(ov8865_setting_SVGA)
	},
	{
		.id = OV8865_MODE_VGA_640_480,
		.hact = 640,
		.htot = 2582,
		.vact = 480,
		.vtot = 1858,
		.reg_data = ov8865_setting_6M,
		.reg_data_size = ARRAY_SIZE(ov8865_setting_6M)
	},
};

static int ov8865_write_reg(struct ov8865_dev *sensor, u16 reg, u8 val)
{
	struct i2c_client *client = sensor->i2c_client;
	struct i2c_msg msg = { 0 };
	u8 buf[3];
	int ret;

	buf[0] = reg >> 8;
	buf[1] = reg & 0xff;
	buf[2] = val;

	msg.addr = client->addr;
	msg.flags = client->flags;
	msg.buf = buf;
	msg.len = sizeof(buf);

	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret < 0) {
		dev_err(&client->dev, "%s: error: reg=%x, val=%x\n",
			__func__, reg, val);
		return ret;
	}

	return 0;
}

static int ov8865_write_reg16(struct ov8865_dev *sensor, u16 reg, u16 val)
{
	int ret;

	ret = ov8865_write_reg(sensor, reg, val >> 8);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, reg + 1, val & 0xff);
	if (ret)
		return ret;

	return 0;
}

static int ov8865_read_reg(struct ov8865_dev *sensor, u16 reg, u8 *val)
{
	struct i2c_client *client = sensor->i2c_client;
	struct i2c_msg msg[2]  = { 0 };
	u8 buf[2];
	int ret = 0;

	buf[0] = reg >> 8;
	buf[1] = reg & 0xff;

	msg[0].addr = client->addr;
	msg[0].flags = client->flags;
	msg[0].buf = buf;
	msg[0].len = sizeof(buf);

	msg[1].addr = client->addr;
	/* Read data from the sensor to the controller */
	msg[1].flags =  I2C_M_RD;
	msg[1].buf = buf;
	msg[1].len = 1;

	ret = i2c_transfer(client->adapter, msg, 2);
	if (ret < 0) {
		dev_err(&client->dev, "%s: error: reg=%x\n", __func__, reg);
		return ret;
	}

	*val = buf[0];

	return 0;
}

static int ov8865_read_reg16(struct ov8865_dev *sensor, u16 reg, u16 *val)
{
	u8 hi, lo;
	int ret;

	ret = ov8865_read_reg(sensor, reg, &hi);
	if (ret)
		return ret;

	ret = ov8865_read_reg(sensor, reg + 1, &lo);
	if (ret)
		return ret;

	*val = ((u16)hi << 8) | (u16)lo;

	return 0;
}

static int ov8865_mod_reg(struct ov8865_dev *sensor, u16 reg, u8 mask, u8 val)
{
	u8 readval;
	int ret;

	ret = ov8865_read_reg(sensor, reg, &readval);
	if (ret)
		return ret;

	readval &= ~mask;
	val &= mask;
	val |= readval;

	ret = ov8865_write_reg(sensor, reg, val);
	if (ret)
		return ret;

	return 0;
}

static int ov8865_set_timings(struct ov8865_dev *sensor,
			      const struct ov8865_mode_info *mode)
{
	int ret;
	u8 isp_y_win_l, x_inc_odd, format2, y_inc_odd,
	   y_inc_even, blc_num_option, zline_num_option,
	   boundary_pix_num;

	ret = ov8865_write_reg(sensor, OV8865_X_ADDR_START_H_REG, 0x00);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_X_ADDR_START_L_REG, 0x0c);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_Y_ADDR_START_H_REG, 0x00);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_Y_ADDR_START_L_REG, 0x0c);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_X_ADDR_END_H_REG, 0x0c);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_X_ADDR_END_L_REG, 0xd3);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_Y_ADDR_END_H_REG, 0x09);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_Y_ADDR_END_L_REG, 0xa3);
	if (ret)
		return ret;

	ret = ov8865_write_reg16(sensor, OV8865_X_OUTPUT_SIZE_REG, mode->hact);
	if (ret)
		return ret;

	ret = ov8865_write_reg16(sensor, OV8865_Y_OUTPUT_SIZE_REG, mode->vact);
	if (ret)
		return ret;

	ret = ov8865_write_reg16(sensor, OV8865_HTS_REG, mode->htot);
	if (ret)
		return ret;

	ret = ov8865_write_reg16(sensor, OV8865_VTS_REG, mode->vtot);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ISP_X_WIN_H_REG, 0x00);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ISP_X_WIN_L_REG, 0x04);
	if (ret)
		return ret;

	if ((mode->id == OV8865_MODE_720P_1280_720) ||
	    (mode->id == OV8865_MODE_UXGA_1600_1200) ||
	    (mode->id == OV8865_MODE_SVGA_800_600)) {
		isp_y_win_l = 0x04;
		x_inc_odd = 0x03;
		blc_num_option = 0x08;
		zline_num_option = 0x02;
		boundary_pix_num = 0x88;

	} else {
		isp_y_win_l = 0x02;
		x_inc_odd = 0x01;
		blc_num_option = 0x04;
		zline_num_option = 0x01;
		boundary_pix_num = 0x48;
	}

	ret = ov8865_write_reg(sensor, OV8865_ISP_Y_WIN_L_REG, isp_y_win_l);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_X_INC_ODD_REG, x_inc_odd);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_X_INC_EVEN_REG, 0x01);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_FORMAT1_REG, 0x00);
	if (ret)
		return ret;

	if ((mode->id == OV8865_MODE_720P_1280_720) ||
	    (mode->id == OV8865_MODE_UXGA_1600_1200)) {
		format2 = 0x67;
		y_inc_odd = 0x03;
	} else if (mode->id == OV8865_MODE_SVGA_800_600) {
		format2 = 0x6f;
		y_inc_odd = 0x05;
	} else {
		format2 = 0x46;
		y_inc_odd = 0x01;
	}

	ret = ov8865_write_reg(sensor, OV8865_FORMAT2_REG, format2);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_Y_INC_ODD_REG, y_inc_odd);
	if (ret)
		return ret;

	if (mode->id == OV8865_MODE_SVGA_800_600)
		y_inc_even = 0x03;
	else
		y_inc_even = 0x01;

	ret = ov8865_write_reg(sensor, OV8865_Y_INC_EVEN_REG, y_inc_even);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_NUM_OPTION_REG,
			       blc_num_option);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ZLINE_NUM_OPTION_REG,
			       zline_num_option);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_RGBC_REG, 0x18);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_AUTO_SIZE_CTRL0_REG, 0xff);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BOUNDARY_PIX_NUM_REG,
			       boundary_pix_num);

	return 0;
}

static int ov8865_get_hts(struct ov8865_dev *sensor)
{
	u16 hts;
	int ret;

	ret = ov8865_read_reg16(sensor, OV8865_HTS_REG, &hts);
	if (ret)
		return ret;
	return hts;
}

static int ov8865_load_regs(struct ov8865_dev *sensor,
			     const struct ov8865_mode_info *mode)
{
	const struct reg_value *regs = mode->reg_data;
	unsigned int i;
	u32 delay_ms = 0;
	u16 reg_addr;
	u8 val;
	int ret = 0;

	for (i = 0; i < mode->reg_data_size; i++, regs++) {
		delay_ms = regs->delay_ms;
		reg_addr = regs->reg_addr;
		val = regs->val;

		ret = ov8865_write_reg(sensor, reg_addr, val);
		if (ret)
			return ret;

		if (delay_ms)
			usleep_range(1000 * delay_ms, 1000 * delay_ms + 100);
	}

	return 0;
}

static const struct ov8865_mode_info *
ov8865_find_mode(struct ov8865_dev *sensor, enum ov8865_frame_rate fr,
		 int width, int height, bool nearest)
{
	const struct ov8865_mode_info *mode;

	mode = v4l2_find_nearest_size(ov8865_mode_data,
				      ARRAY_SIZE(ov8865_mode_data),
				      hact, vact, width, height);

	if (!mode || (!nearest && (mode->hact != width || mode->vact !=
				   height)))
		return NULL;

	/* Only SVGA can operate 90 fps. */
	if (fr == OV8865_90_FPS && !(mode->hact == 800 && mode->vact == 600))
		return NULL;

	return mode;
}

static u64 ov8865_calc_pixel_rate(struct ov8865_dev *sensor)
{
	u64 rate;

	rate = sensor->current_mode->vtot * sensor->current_mode->htot;
	rate *= ov8865_framerates[sensor->current_fr];

	return rate;
}

static int ov8865_set_mode_direct(struct ov8865_dev *sensor,
			      const struct ov8865_mode_info *mode)
{
	int ret;

	if (!mode->reg_data)
		return -EINVAL;

	/*Write capture setting*/
	ret = ov8865_load_regs(sensor, mode);
	if (ret)
		return ret;

	return 0;
}

static int ov8865_set_black_level(struct ov8865_dev *sensor)
{
	const struct ov8865_mode_info *mode = sensor->current_mode;
	int ret;
	u8 blc_ctrl1, left_start_h, left_start_l, left_end_h,
	   left_end_l, right_start_h, right_start_l,
	   right_end_h, right_end_l, bkline_num, bkline_st,
	   zline_st, zline_num, blkline_st;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRL0_REG, 0xf1);
	if (ret)
		return ret;

	if ((mode->id == OV8865_MODE_QUXGA_3264_2448) ||
	    (mode->id == OV8865_MODE_6M_3264_1836) ||
	    (mode->id == OV8865_MODE_1080P_1920_1080) ||
	    (mode->id == OV8865_MODE_VGA_640_480))
		blc_ctrl1 = 0x04;
	else
		blc_ctrl1 = 0x14;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRL1_REG, blc_ctrl1);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRL5_REG, 0x10);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRLB_REG, 0x0c);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRLD_REG, 0x10);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRL1B_REG, 0x00);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRL1D_REG, 0x00);
	if (ret)
		return ret;

	if ((mode->id  == OV8865_MODE_QUXGA_3264_2448) ||
	    (mode->id  == OV8865_MODE_6M_3264_1836) ||
	    (mode->id == OV8865_MODE_1080P_1920_1080) ||
	    (mode->id == OV8865_MODE_VGA_640_480)) {
		left_start_h = 0x02;
		left_start_l = 0x40;
		left_end_h = 0x03;
		left_end_l = 0x3f;
		right_start_h = 0x07;
		right_start_l = 0xc0;
		right_end_h = 0x08;
		right_end_l = 0xbf;
	} else {
		left_start_h = 0x01;
		left_start_l = 0x20;
		left_end_h = 0x01;
		left_end_l = 0x9f;
		right_start_h = 0x03;
		right_start_l = 0xe0;
		right_end_h = 0x04;
		right_end_l = 0x5f;
	}

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_LEFT_START_H_REG,
			       left_start_h);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_LEFT_START_L_REG,
			       left_start_l);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_LEFT_END_H_REG,
			       left_end_h);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_LEFT_END_L_REG,
			       left_end_l);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_RIGHT_START_H_REG,
			       right_start_h);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_RIGHT_START_L_REG,
			       right_start_l);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_RIGHT_END_H_REG,
			       right_end_h);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_ANCHOR_RIGHT_END_L_REG,
			       right_end_l);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_TOP_ZLINE_ST_REG, 0x00);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_TOP_ZLINE_NUM_REG, 0x02);
	if (ret)
		return ret;

	if (mode->id == OV8865_MODE_SVGA_800_600) {
		bkline_st = 0x02;
		bkline_num = 0x02;
		zline_st = 0x00;
		zline_num = 0x00;
		blkline_st = 0x04;
	} else {
		bkline_st = 0x04;
		bkline_num = 0x04;
		zline_st = 0x02;
		zline_num = 0x02;
		blkline_st = 0x08;
	}
	ret = ov8865_write_reg(sensor, OV8865_TOP_BKLINE_ST_REG, bkline_st);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_TOP_BKLINE_NUM_REG, bkline_num);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BOT_ZLINE_ST_REG, zline_st);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BOT_ZLINE_NUM_REG, zline_num);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BOT_BLKLINE_ST_REG, blkline_st);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BOT_BLKLINE_NUM_REG, 0x02);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_CTRL1F_REG, 0x00);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_BLC_OFFSET_LIMIT_REG, 0x3f);
	if (ret)
		return ret;

	return 0;
}

static int ov8865_set_pclk(struct ov8865_dev *sensor)
{
	int ret;

	ret = ov8865_write_reg(sensor, OV8865_PLL_CTRL2_REG, 0x1e);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_PLL_CTRL3_REG, 0x00);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_PLL_CTRL4_REG, 0x03);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_CLOCK_SEL_REG, 0x93);
	if (ret)
		return ret;

	return 0;
}

static int ov8865_get_pclk(struct ov8865_dev *sensor)
{
	int ret;
	u8 pll1_mult, m_div, mipi_div_r, mipi_div, pclk_div_r, pclk_div;
	int ref_clk = sensor->xclk_freq / 1000000;

	ret = ov8865_read_reg(sensor, OV8865_PLL_CTRL2_REG, &pll1_mult);
	if (ret)
		return ret;

	ret = ov8865_read_reg(sensor, OV8865_PLL_CTRL3_REG, &m_div);
	if (ret)
		return ret;

	m_div = m_div & 0x07;
	ret = ov8865_read_reg(sensor, OV8865_PLL_CTRL4_REG, &mipi_div_r);
	if (ret)
		return ret;

	mipi_div_r = mipi_div_r & 0x03;

	if (mipi_div_r == 0x00)
		mipi_div = 4;

	if (mipi_div_r == 0x01)
		mipi_div = 5;

	if (mipi_div_r == 0x02)
		mipi_div = 6;

	if (mipi_div_r == 0x03)
		mipi_div = 8;

	ret = ov8865_read_reg(sensor,  OV8865_CLOCK_SEL_REG, &pclk_div_r);
	if (ret)
		return ret;

	pclk_div_r = (pclk_div_r & 0x08) >> 3;

	if (pclk_div_r == 0)
		pclk_div = 1;

	if (pclk_div_r == 1)
		pclk_div = 2;

	return ref_clk * pll1_mult / (1 + m_div) / mipi_div / pclk_div;
}

static int ov8865_set_sclk(struct ov8865_dev *sensor)
{
	const struct ov8865_mode_info *mode = sensor->current_mode;
	int ret;
	u8 val;

	if ((mode->id  == OV8865_MODE_UXGA_1600_1200) ||
	    (mode->id == OV8865_MODE_720P_1280_720) ||
	    (mode->id == OV8865_MODE_SVGA_800_600))
		val = 0x09;
	else
		val = 0x04;

	ret = ov8865_write_reg(sensor, OV8865_PLL_CTRLF_REG, val);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_PLL_CTRL12_REG, 0x01);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_PLL_CTRL1E_REG, 0x0c);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_PLL_CTRLE_REG, 0x00);
	if (ret)
		return ret;

	return 0;
}

static int ov8865_set_virtual_channel(struct ov8865_dev *sensor, u8 channel)
{
	u8 channel_id;
	int ret;

	ret = ov8865_read_reg(sensor, OV8865_MIPI_CTRL13_REG, &channel_id);
	if (ret)
		return ret;

	ret = ov8865_write_reg(sensor, OV8865_MIPI_CTRL13_REG, channel_id |
				channel);
	if (ret)
		return ret;

	return 0;
}

static int ov8865_set_mode(struct ov8865_dev *sensor)
{
	const struct ov8865_mode_info *mode = sensor->current_mode;
	int ret;

	ret = ov8865_set_pclk(sensor);
	if (ret < 0)
		return ret;

	ret = ov8865_set_sclk(sensor);
	if (ret < 0)
		return ret;

	ret = ov8865_set_black_level(sensor);
	if (ret)
		return ret;

	ret = ov8865_set_timings(sensor, mode);
	if (ret)
		return ret;

	ret = ov8865_set_mode_direct(sensor, mode);
	if (ret < 0)
		return ret;

	ret = ov8865_set_virtual_channel(sensor, 0);
	if (ret < 0)
		return ret;

	sensor->last_mode = mode;
	return 0;
}

static int ov8865_restore_mode(struct ov8865_dev *sensor)
{
	int ret;

	ret = ov8865_load_regs(sensor, &ov8865_mode_init_data);
	if (ret)
		return ret;

	sensor->last_mode = &ov8865_mode_init_data;

	ret = ov8865_set_mode(sensor);
	if (ret)
		return ret;

	return 0;
}

static void ov8865_power(struct ov8865_dev *sensor, bool enable)
{
	gpiod_set_value_cansleep(sensor->pwdn_gpio, enable ? 0 : 1);
}

static void ov8865_reset(struct ov8865_dev *sensor, bool enable)
{
	gpiod_set_value_cansleep(sensor->reset_gpio, enable ? 0 : 1);
}

static int ov8865_set_power_on(struct ov8865_dev *sensor)
{
	struct i2c_client *client = sensor->i2c_client;
	int ret = 0;

	ov8865_power(sensor, false);
	ov8865_reset(sensor, false);

	ret = clk_prepare_enable(sensor->xclk);
	if (ret) {
		dev_err(&client->dev, "%s: failed to enable clock\n",
			__func__);
		return ret;
	}

	ov8865_power(sensor, true);

	ret = regulator_bulk_enable(OV8865_NUM_SUPPLIES, sensor->supplies);
	if (ret) {
		dev_err(&client->dev, "%s: failed to enable regulators\n",
			__func__);
		goto err_power_off;
	}

	ov8865_reset(sensor, true);
	usleep_range(10000, 12000);

	return 0;

err_power_off:
	ov8865_power(sensor, false);
	clk_disable_unprepare(sensor->xclk);
	return ret;
}

static void ov8865_set_power_off(struct ov8865_dev *sensor)
{
	ov8865_power(sensor, false);
	regulator_bulk_disable(OV8865_NUM_SUPPLIES, sensor->supplies);
	clk_disable_unprepare(sensor->xclk);
}

static int ov8865_set_power(struct ov8865_dev *sensor, bool on)
{
	int ret = 0;

	if (on) {
		ret = ov8865_set_power_on(sensor);
		if (ret)
			return ret;

		ret = ov8865_restore_mode(sensor);
		if (ret)
			goto err_power_off;
	} else {
		ov8865_set_power_off(sensor);
	}

	return 0;

err_power_off:
	ov8865_set_power_off(sensor);
	return ret;
}

static int ov8865_s_power(struct v4l2_subdev *sd, int on)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	int ret = 0;

	mutex_lock(&sensor->lock);
	if (sensor->power_count == !on) {
		ret = ov8865_set_power(sensor, !!on);
		if (ret)
			goto out;
	}

	/* Update the power count. */
	sensor->power_count += on ? 1 : -1;
	WARN_ON(sensor->power_count < 0);
out:
	mutex_unlock(&sensor->lock);

	if (on && !ret && sensor->power_count == 1) {
		/* Initialize the hardware. */
		ret = v4l2_ctrl_handler_setup(&sensor->ctrls.handler);
	}

	return ret;
}

static int ov8865_try_frame_interval(struct ov8865_dev *sensor,
				     struct v4l2_fract *fi,
				     u32 width, u32 height)
{
	const struct ov8865_mode_info *mode;
	enum ov8865_frame_rate rate = OV8865_30_FPS;
	int minfps, maxfps, best_fps, fps;
	int i;

	minfps = ov8865_framerates[OV8865_30_FPS];
	maxfps = ov8865_framerates[OV8865_90_FPS];

	if (fi->numerator == 0) {
		fi->denominator = maxfps;
		fi->numerator = 1;
		rate = OV8865_90_FPS;
		goto find_mode;
	}

	fps = clamp_val(DIV_ROUND_CLOSEST(fi->denominator, fi->numerator),
			minfps, maxfps);

	best_fps = minfps;
	for (i = 0; i < ARRAY_SIZE(ov8865_framerates); i++) {
		int curr_fps = ov8865_framerates[i];

		if (abs(curr_fps - fps) < abs(best_fps - fps)) {
			best_fps = curr_fps;
			rate = i;
		}
	}

	fi->numerator = 1;
	fi->denominator = best_fps;

find_mode:
	mode = ov8865_find_mode(sensor, rate, width, height, false);

	return mode ? rate : -EINVAL;
}

static int ov8865_try_fmt_internal(struct v4l2_subdev *sd,
				   struct v4l2_mbus_framefmt *fmt,
				   enum ov8865_frame_rate fr,
				   const struct ov8865_mode_info **new_mode)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	const struct ov8865_mode_info *mode;
	int i;

	mode = ov8865_find_mode(sensor, fr, fmt->width, fmt->height, true);
	if (!mode)
		return -EINVAL;

	fmt->width = mode->hact;
	fmt->height = mode->vact;

	if (new_mode)
		*new_mode = mode;

	for (i = 0; i < ARRAY_SIZE(ov8865_formats); i++)
		if (ov8865_formats[i].code == fmt->code)
			break;

	if (i == ARRAY_SIZE(ov8865_formats))
		i = 0;

	fmt->code = ov8865_formats[i].code;
	fmt->colorspace = ov8865_formats[i].colorspace;
	fmt->ycbcr_enc = V4L2_MAP_YCBCR_ENC_DEFAULT(fmt->colorspace);
	fmt->quantization = V4L2_QUANTIZATION_FULL_RANGE;
	fmt->xfer_func = V4L2_MAP_XFER_FUNC_DEFAULT(fmt->colorspace);

	return 0;
}

static int ov8865_get_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *format)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	struct v4l2_mbus_framefmt *fmt;

	if (format->pad != 0)
		return -EINVAL;

	mutex_lock(&sensor->lock);
	if (format->which == V4L2_SUBDEV_FORMAT_TRY)
		fmt = v4l2_subdev_get_try_format(&sensor->sd, cfg,
						 format->pad);
	else
		fmt = &sensor->fmt;

	if (fmt)
		format->format = *fmt;

	mutex_unlock(&sensor->lock);

	return fmt ? 0 : -EINVAL;
}

static int ov8865_set_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *format)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	const struct ov8865_mode_info *new_mode;
	struct v4l2_mbus_framefmt *mbus_fmt = &format->format;
	struct v4l2_mbus_framefmt *fmt;
	int ret;

	if (format->pad != 0)
		return -EINVAL;

	mutex_lock(&sensor->lock);

	if (sensor->streaming) {
		ret = -EBUSY;
		goto out;
	}

	ret = ov8865_try_fmt_internal(sd, mbus_fmt, sensor->current_fr,
				      &new_mode);
	if (ret)
		goto out;

	if (format->which == V4L2_SUBDEV_FORMAT_TRY)
		fmt = v4l2_subdev_get_try_format(sd, cfg, 0);
	else
		fmt = &sensor->fmt;

	if (fmt)
		*fmt = *mbus_fmt;
	else
		ret = -EINVAL;

	if (new_mode != sensor->current_mode)
		sensor->current_mode = new_mode;

	__v4l2_ctrl_s_ctrl_int64(sensor->ctrls.pixel_rate,
				 ov8865_calc_pixel_rate(sensor));

out:
	mutex_unlock(&sensor->lock);
	return ret;
}

static int ov8865_set_ctrl_hflip(struct ov8865_dev *sensor, int value)
{
	return ov8865_mod_reg(sensor, OV8865_FORMAT2_REG,
			      OV8865_FORMAT2_MIRROR_DIG |
			      OV8865_FORMAT2_MIRROR_ARR,
			      (!(value ^ sensor->upside_down)) ?
			      (OV8865_FORMAT2_MIRROR_DIG |
			       OV8865_FORMAT2_MIRROR_ARR) : 0);
}

static int ov8865_set_ctrl_vflip(struct ov8865_dev *sensor, int value)
{
	return ov8865_mod_reg(sensor, OV8865_FORMAT1_REG,
			      OV8865_FORMAT1_MIRROR_DIG |
			      OV8865_FORMAT1_MIRROR_ARR,
			      (value ^ sensor->upside_down) ?
			      (OV8865_FORMAT2_MIRROR_DIG |
			       OV8865_FORMAT2_MIRROR_ARR) : 0);
}

static int ov8865_get_exposure(struct ov8865_dev *sensor)
{
	int exp, ret, pclk, hts, line_time;
	u8 temp;

	ret = ov8865_read_reg(sensor, OV8865_EXPOSURE_CTRL_HH_REG, &temp);
	if (ret)
		return ret;
	exp = ((int)temp & 0x0f) << 16;

	ret = ov8865_read_reg(sensor, OV8865_EXPOSURE_CTRL_H_REG, &temp);
	if (ret)
		return ret;
	exp |= ((int)temp << 8);

	ret = ov8865_read_reg(sensor, OV8865_EXPOSURE_CTRL_L_REG, &temp);
	if (ret)
		return ret;
	exp |= (int)temp;

	ret = ov8865_get_pclk(sensor);
	if (ret <= 0)
		return ret;

	pclk = ret;

	ret = ov8865_get_hts(sensor);
	if (ret <= 0)
		return ret;

	hts = ret;

	line_time = hts / pclk;

	/* The low 4 bits of exposure are the fractional part. And the unit is
	 * 1/16 of a line lecture time. The pclk and HTS are used to calculate
	 * this time. For V4L2, the value 1 of exposure stands for 100us of
	 * capture.
	 */
	return (exp >> 4) * line_time / 16 / 100;
}

static int ov8865_get_gain(struct ov8865_dev *sensor)
{
	u16 gain;
	int ret;

	/* Linear gain. */
	ret = ov8865_read_reg16(sensor, OV8865_GAIN_CTRL_H_REG, &gain);
	if (ret)
		return ret;

	return gain & 0x1fff;
}

static int ov8865_set_ctrl_exp(struct ov8865_dev *sensor)
{
	struct ov8865_ctrls *ctrls = &sensor->ctrls;
	int ret = 0, hts, pclk, line_time;
	int exposure = ctrls->exposure->val;
	/* The low 4 bits of exposure are the fractional part. And the unit is
	 * 1/16 of a line lecture time. The pclk and HTS are used to calculate
	 * this time. For V4L2, the value 1 of exposure stands for 100us of
	 * capture.
	 */

	ret = ov8865_get_pclk(sensor);
	if (ret <= 0)
		return ret;
	pclk = ret;

	ret = ov8865_get_hts(sensor);
	if (ret <= 0)
		return ret;
	hts = ret;

	line_time = hts / pclk;

	exposure = ctrls->exposure->val * 16 / line_time * 100;
	exposure = (exposure << 4);

	if (ctrls->exposure->is_new) {
		ret = ov8865_write_reg(sensor, OV8865_EXPOSURE_CTRL_L_REG,
				       exposure & 0xff);
		if (ret)
			return ret;

		ret = ov8865_write_reg(sensor, OV8865_EXPOSURE_CTRL_H_REG,
				       (exposure >> 8) & 0xff);
		if (ret)
			return ret;

		ret = ov8865_write_reg(sensor, OV8865_EXPOSURE_CTRL_HH_REG,
				       (exposure >> 16) & 0x0f);
	}

	return ret;
}

static int ov8865_set_ctrl_gain(struct ov8865_dev *sensor)
{
	struct ov8865_ctrls *ctrls = &sensor->ctrls;
	int ret = 0;
	int val = ctrls->gain->val;

	/* Linear gain. */
	if (ctrls->gain->is_new)
		ret = ov8865_write_reg16(sensor, OV8865_GAIN_CTRL_H_REG,
					 (u16)val & 0x1fff);
	return ret;
}

static int ov8865_g_volatile_ctrl(struct v4l2_ctrl *ctrl)
{
	struct v4l2_subdev *sd = ctrl_to_sd(ctrl);
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	int val;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		val = ov8865_get_gain(sensor);
		if (val < 0)
			return val;
		sensor->ctrls.gain->val = val;
		break;
	case V4L2_CID_EXPOSURE:
		val = ov8865_get_exposure(sensor);
		if (val < 0)
			return val;
		sensor->ctrls.exposure->val = val;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static int ov8865_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct v4l2_subdev *sd = ctrl_to_sd(ctrl);
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	int ret;

	if (sensor->power_count == 0)
		return 0;

	switch (ctrl->id) {
	case V4L2_CID_GAIN:
		ret = ov8865_set_ctrl_gain(sensor);
		break;
	case V4L2_CID_EXPOSURE:
		ret = ov8865_set_ctrl_exp(sensor);
		break;
	case V4L2_CID_HFLIP:
		ret = ov8865_set_ctrl_hflip(sensor, ctrl->val);
		break;
	case V4L2_CID_VFLIP:
		ret = ov8865_set_ctrl_vflip(sensor, ctrl->val);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static const struct v4l2_ctrl_ops ov8865_ctrl_ops = {
	.g_volatile_ctrl = ov8865_g_volatile_ctrl,
	.s_ctrl = ov8865_s_ctrl,
};

static int ov8865_init_controls(struct ov8865_dev *sensor)
{
	const struct v4l2_ctrl_ops *ops = &ov8865_ctrl_ops;
	struct ov8865_ctrls *ctrls = &sensor->ctrls;
	struct v4l2_ctrl_handler *hdl = &ctrls->handler;
	int ret;

	v4l2_ctrl_handler_init(hdl, 32);
	hdl->lock = &sensor->lock;
	ctrls->pixel_rate = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_PIXEL_RATE,
					      0, INT_MAX, 1,
					      ov8865_calc_pixel_rate(sensor));
	ctrls->exposure = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_EXPOSURE, 1,
					    2000, 1, 1);
	ctrls->gain = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_GAIN, 1*16, 64*16 - 1,
					1, 1*16);
	ctrls->hflip = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_HFLIP, 0, 1, 1, 0);
	ctrls->vflip = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_VFLIP, 0, 1, 1, 0);
	if (hdl->error) {
		ret = hdl->error;
		goto err_free_ctrls;
	}

	ctrls->pixel_rate->flags |= V4L2_CTRL_FLAG_READ_ONLY;

	sensor->sd.ctrl_handler = hdl;

	return 0;

err_free_ctrls:
	v4l2_ctrl_handler_free(hdl);
	return ret;
}


static int ov8865_enum_frame_size(struct v4l2_subdev *sd,
				  struct v4l2_subdev_pad_config *cfg,
				  struct v4l2_subdev_frame_size_enum *fse)
{

	if (fse->pad != 0 || fse->index >= OV8865_NUM_MODES)
		return -EINVAL;

	fse->code = MEDIA_BUS_FMT_SBGGR10_1X10;
	fse->min_width = ov8865_mode_data[fse->index].hact;
	fse->max_width = fse->min_width;
	fse->min_height = ov8865_mode_data[fse->index].vact;
	fse->max_height = fse->min_height;

	return 0;
}

static int ov8865_enum_frame_interval(struct v4l2_subdev *sd,
				      struct v4l2_subdev_pad_config *cfg,
				      struct v4l2_subdev_frame_interval_enum
				      *fie)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	struct v4l2_fract tpf;
	int ret;

	if (fie->pad != 0 || fie->index >= OV8865_NUM_FRAMERATES)
		return -EINVAL;

	tpf.numerator = 1;
	tpf.denominator = ov8865_framerates[fie->index];

	ret = ov8865_try_frame_interval(sensor, &tpf,
					fie->width, fie->height);
	if (ret < 0)
		return -EINVAL;

	fie->interval = tpf;

	return 0;
}

static int ov8865_g_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *fi)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);

	mutex_lock(&sensor->lock);
	fi->interval = sensor->frame_interval;
	mutex_unlock(&sensor->lock);

	return 0;
}

static int ov8865_s_frame_interval(struct v4l2_subdev *sd,
				   struct v4l2_subdev_frame_interval *fi)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	const struct ov8865_mode_info *mode;
	int frame_rate, ret = 0;

	if (fi->pad != 0)
		return -EINVAL;

	mutex_lock(&sensor->lock);

	if (sensor->streaming) {
		ret = -EBUSY;
		goto out;
	}

	mode = sensor->current_mode;

	frame_rate = ov8865_try_frame_interval(sensor, &fi->interval,
					       mode->hact, mode->vact);
	if (frame_rate < 0) {
		fi->interval = sensor->frame_interval;
		goto out;
	}

	mode = ov8865_find_mode(sensor, frame_rate, mode->hact,
				 mode->vact, true);
	if (!mode) {
		ret = -EINVAL;
		goto out;
	}

	if (mode != sensor->current_mode ||
	    frame_rate != sensor->current_fr) {
		sensor->current_fr = frame_rate;
		sensor->frame_interval = fi->interval;
		sensor->current_mode = mode;

		__v4l2_ctrl_s_ctrl_int64(sensor->ctrls.pixel_rate,
					 ov8865_calc_pixel_rate(sensor));
	}

out:
	mutex_unlock(&sensor->lock);
	return ret;
}

static int ov8865_enum_mbus_code(struct v4l2_subdev *sd,
				 struct v4l2_subdev_pad_config *cfg,
				 struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->pad != 0 || code->index >= ARRAY_SIZE(ov8865_formats))
		return -EINVAL;

	code->code = ov8865_formats[code->index].code;

	return 0;
}

static int ov8865_s_stream(struct v4l2_subdev *sd, int enable)
{
	struct ov8865_dev *sensor = to_ov8865_dev(sd);
	int ret = 0;

	mutex_lock(&sensor->lock);

	if (sensor->streaming == !enable) {
		if (enable && ret)
			goto out;

		ret = ov8865_write_reg(sensor, OV8865_SW_STANDBY_REG, enable ?
				     OV8865_SW_STANDBY_STANDBY_N : 0x00);
		if (ret)
			return ret;

		ret = ov8865_write_reg(sensor, OV8865_MIPI_CTRL_REG,
				       enable ? 0x72 : 0x62);
		if (ret)
			goto out;

		if (!ret)
			sensor->streaming = enable;
	}

out:
	mutex_unlock(&sensor->lock);
	return ret;
}

static const struct v4l2_subdev_core_ops ov8865_core_ops = {
	.s_power = ov8865_s_power,
	.log_status = v4l2_ctrl_subdev_log_status,
	.subscribe_event = v4l2_ctrl_subdev_subscribe_event,
	.unsubscribe_event = v4l2_event_subdev_unsubscribe,
};

static const struct v4l2_subdev_video_ops ov8865_video_ops = {
	.g_frame_interval = ov8865_g_frame_interval,
	.s_frame_interval = ov8865_s_frame_interval,
	.s_stream = ov8865_s_stream,
};

static const struct v4l2_subdev_pad_ops ov8865_pad_ops = {
	.enum_mbus_code = ov8865_enum_mbus_code,
	.get_fmt = ov8865_get_fmt,
	.set_fmt = ov8865_set_fmt,
	.enum_frame_size = ov8865_enum_frame_size,
	.enum_frame_interval = ov8865_enum_frame_interval,
};

static const struct v4l2_subdev_ops ov8865_subdev_ops = {
	.core = &ov8865_core_ops,
	.video = &ov8865_video_ops,
	.pad = &ov8865_pad_ops,
};

static int ov8865_get_regulators(struct ov8865_dev *sensor)
{
	int i;

	for (i = 0; i < OV8865_NUM_SUPPLIES; i++)
		sensor->supplies[i].supply = ov8865_supply_names[i];

	return devm_regulator_bulk_get(&sensor->i2c_client->dev,
				       OV8865_NUM_SUPPLIES,
				       sensor->supplies);
}

static int ov8865_check_chip_id(struct ov8865_dev *sensor)
{
	struct i2c_client *client = sensor->i2c_client;
	int ret = 0;
	u8 chip_id_0, chip_id_1, chip_id_2;
	u32 chip_id = 0x000000;

	ret = ov8865_set_power_on(sensor);
	if (ret)
		return ret;

	ret = ov8865_read_reg(sensor, OV8865_CHIP_ID_REG, &chip_id_0);
	if (ret) {
		dev_err(&client->dev, "%s: failed to reach chip identifier\n",
			__func__);
		goto power_off;
	}

	ret = ov8865_read_reg(sensor, OV8865_CHIP_ID_REG + 1, &chip_id_1);
	if (ret) {
		dev_err(&client->dev, "%s: failed to reach chip identifier\n",
			__func__);
		goto power_off;
	}

	ret = ov8865_read_reg(sensor, OV8865_CHIP_ID_REG + 2, &chip_id_2);
	if (ret) {
		dev_err(&client->dev, "%s: failed to reach chip identifier\n",
			__func__);
		goto power_off;
	}

	chip_id = ((u32)chip_id_0 << 16) | ((u32)chip_id_1 << 8) |
		((u32)chip_id_2);

	if (chip_id != OV8865_CHIP_ID) {
		dev_err(&client->dev, "%s: wrong chip identifier, expected 0x008865, got 0x%x\n", __func__, chip_id);
		ret = -ENXIO;
	}

power_off:
	ov8865_set_power_off(sensor);
	return ret;
}

static int ov8865_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct fwnode_handle *endpoint;
	struct ov8865_dev *sensor;
	const struct ov8865_mode_info *default_mode;
	struct v4l2_mbus_framefmt *fmt;
	u32 rotation;
	int ret = 0;

	sensor = devm_kzalloc(dev, sizeof(*sensor), GFP_KERNEL);
	if (!sensor)
		return -ENOMEM;

	sensor->i2c_client = client;

	/*
	 * Default init sequence initialize sensor to
	 * RAW SBGGR10 3264x1836@30fps.
	 */

	default_mode = &ov8865_mode_data[OV8865_MODE_QUXGA_3264_2448];

	fmt = &sensor->fmt;
	fmt->code = MEDIA_BUS_FMT_SBGGR10_1X10;
	fmt->colorspace = V4L2_COLORSPACE_RAW;
	fmt->ycbcr_enc = V4L2_MAP_YCBCR_ENC_DEFAULT(fmt->colorspace);
	fmt->quantization = V4L2_QUANTIZATION_FULL_RANGE;
	fmt->xfer_func = V4L2_MAP_XFER_FUNC_DEFAULT(fmt->colorspace);
	fmt->width = default_mode->hact;
	fmt->height = default_mode->vact;
	fmt->field = V4L2_FIELD_NONE;
	sensor->frame_interval.numerator = 1;
	sensor->frame_interval.denominator = ov8865_framerates[OV8865_30_FPS];
	sensor->current_fr = OV8865_30_FPS;
	sensor->current_mode = default_mode;
	sensor->last_mode = default_mode;

	/* Optional indication of physical rotation of sensor. */
	ret = fwnode_property_read_u32(dev_fwnode(&client->dev), "rotation",
				       &rotation);
	if (!ret) {
		switch (rotation) {
		case 180:
			sensor->upside_down = true;
			/* fall through */
		case 0:
			break;
		default:
			dev_warn(dev, "%u degrees rotation is not supported, ignoring..\n",
				 rotation);
		}
	}

	endpoint = fwnode_graph_get_next_endpoint(dev_fwnode(&client->dev),
						  NULL);
	if (!endpoint) {
		dev_err(dev, "endpoint node not found\n");
		return -EINVAL;
	}

	ret = v4l2_fwnode_endpoint_parse(endpoint, &sensor->ep);
	fwnode_handle_put(endpoint);
	if (ret) {
		dev_err(dev, "Could not parse endpoint\n");
		return ret;
	}

	/* Get system clock (xclk). */
	sensor->xclk = devm_clk_get(dev, "xclk");
	if (IS_ERR(sensor->xclk)) {
		dev_err(dev, "failed to get xclk\n");
		return PTR_ERR(sensor->xclk);
	}

	sensor->xclk_freq = clk_get_rate(sensor->xclk);
	if (sensor->xclk_freq != 24000000) {
		dev_err(dev, "xclk frequency out of range: %d Hz, it should be 24000000 Hz\n",
			sensor->xclk_freq);
		return -EINVAL;
	}
	/* Request optional power down pin. */
	sensor->pwdn_gpio = devm_gpiod_get_optional(dev, "powerdown",
						    GPIOD_OUT_HIGH);
	if (IS_ERR(sensor->pwdn_gpio))
		return PTR_ERR(sensor->pwdn_gpio);

	/* Request optional reset pin. */
	sensor->reset_gpio = devm_gpiod_get_optional(dev, "reset",
						     GPIOD_OUT_HIGH);
	if (IS_ERR(sensor->reset_gpio))
		return PTR_ERR(sensor->reset_gpio);

	v4l2_i2c_subdev_init(&sensor->sd, client, &ov8865_subdev_ops);
	sensor->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE |
			    V4L2_SUBDEV_FL_HAS_EVENTS;
	sensor->pad.flags = MEDIA_PAD_FL_SOURCE;
	sensor->sd.entity.function = MEDIA_ENT_F_CAM_SENSOR;
	ret = media_entity_pads_init(&sensor->sd.entity, 1, &sensor->pad);
	if (ret)
		return ret;

	ret = ov8865_get_regulators(sensor);
	if (ret)
		return ret;

	mutex_init(&sensor->lock);

	ret = ov8865_check_chip_id(sensor);
	if (ret)
		goto err_entity_cleanup;

	ret = ov8865_init_controls(sensor);
	if (ret)
		goto err_entity_cleanup;

	ret = v4l2_async_register_subdev(&sensor->sd);
	if (ret)
		goto err_free_ctrls;

	return 0;

err_free_ctrls:
	v4l2_ctrl_handler_free(&sensor->ctrls.handler);
err_entity_cleanup:
	mutex_destroy(&sensor->lock);
	media_entity_cleanup(&sensor->sd.entity);
	return ret;
}


static int ov8865_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct ov8865_dev *sensor = to_ov8865_dev(sd);

	v4l2_async_unregister_subdev(&sensor->sd);
	mutex_destroy(&sensor->lock);
	media_entity_cleanup(&sensor->sd.entity);
	v4l2_ctrl_handler_free(&sensor->ctrls.handler);

	return 0;
}

static const struct i2c_device_id ov8865_id[] = {
	{ "ov8865", 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, ov8865_id);

static const struct of_device_id ov8865_dt_ids[] = {
	{ .compatible = "ovti,ov8865" },
	{ }
};
MODULE_DEVICE_TABLE(of, ov8865_dt_ids);

static struct i2c_driver ov8865_i2c_driver = {
	.driver	= {
		 .name = "ov8865",
		 .of_match_table = ov8865_dt_ids,
	 },
	 .id_table     = ov8865_id,
	 .probe_new    = ov8865_probe,
	 .remove       = ov8865_remove,
};

module_i2c_driver(ov8865_i2c_driver);

MODULE_DESCRIPTION("OV8865 MIPI Camera Subdev Driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kévin L'hôpital <kevin.lhopital@bootlin.com>");
