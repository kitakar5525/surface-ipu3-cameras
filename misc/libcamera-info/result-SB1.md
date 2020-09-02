The following is the result with commit https://github.com/kitakar5525/surface-ipu3-cameras/tree/1fd8c87fceb10f6228635ec670cac496c3d1d48e.
- Using DSDT-override and not using surface_camera driver
- Loaded ov5693, ov7251, and ov8865 sensor drivers

```bash
$ sudo bash ./libcamera-info.sh
v4l-subdev0: ipu3-imgu 0
v4l-subdev1: ipu3-imgu 1
v4l-subdev2: ipu3-csi2 0
v4l-subdev3: ipu3-csi2 1
v4l-subdev4: ipu3-csi2 2
v4l-subdev5: ipu3-csi2 3
v4l-subdev6: ov5693 2-0036
v4l-subdev7: ov7251 3-0060
v4l-subdev8: ov8865 3-0010
video0: ipu3-cio2 0
video1: ipu3-cio2 1
video2: ipu3-cio2 2
video3: ipu3-cio2 3
video4: ipu3-imgu 0 input
video5: ipu3-imgu 0 parameters
video6: ipu3-imgu 0 output
video7: ipu3-imgu 0 viewfinder
video8: ipu3-imgu 0 3a stat
video9: ipu3-imgu 1 input
video10: ipu3-imgu 1 parameters
video11: ipu3-imgu 1 output
video12: ipu3-imgu 1 viewfinder
video13: ipu3-imgu 1 3a stat
media0: Intel IPU3 CIO2
media1: ipu3-imgu
/dev/media0:
Media controller API version 5.8.4

Media device information
------------------------
driver          ipu3-cio2
model           Intel IPU3 CIO2
serial          
bus info        PCI:0000:00:14.3
hw revision     0x0
driver version  5.8.4

Device topology
- entity 1: ipu3-csi2 0 (2 pads, 2 links)
            type V4L2 subdev subtype Unknown flags 0
            device node name /dev/v4l-subdev2
	pad0: Sink
		<- "ov8865 3-0010":0 [ENABLED]
	pad1: Source
		[fmt:SGRBG10_1X10/1936x1096 field:none]
		-> "ipu3-cio2 0":0 [ENABLED,IMMUTABLE]

- entity 4: ipu3-cio2 0 (1 pad, 1 link)
            type Node subtype V4L flags 0
            device node name /dev/video0
	pad0: Sink
		<- "ipu3-csi2 0":1 [ENABLED,IMMUTABLE]

- entity 10: ipu3-csi2 1 (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev3
	pad0: Sink
		<- "ov5693 2-0036":0 [ENABLED]
	pad1: Source
		[fmt:SGRBG10_1X10/1936x1096 field:none]
		-> "ipu3-cio2 1":0 [ENABLED,IMMUTABLE]

- entity 13: ipu3-cio2 1 (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video1
	pad0: Sink
		<- "ipu3-csi2 1":1 [ENABLED,IMMUTABLE]

- entity 19: ipu3-csi2 2 (2 pads, 2 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev4
	pad0: Sink
		<- "ov7251 3-0060":0 []
	pad1: Source
		[fmt:SGRBG10_1X10/1936x1096 field:none]
		-> "ipu3-cio2 2":0 [ENABLED,IMMUTABLE]

- entity 22: ipu3-cio2 2 (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video2
	pad0: Sink
		<- "ipu3-csi2 2":1 [ENABLED,IMMUTABLE]

- entity 28: ipu3-csi2 3 (2 pads, 1 link)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev5
	pad0: Sink
	pad1: Source
		[fmt:SGRBG10_1X10/1936x1096 field:none]
		-> "ipu3-cio2 3":0 [ENABLED,IMMUTABLE]

- entity 31: ipu3-cio2 3 (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video3
	pad0: Sink
		<- "ipu3-csi2 3":1 [ENABLED,IMMUTABLE]

- entity 37: ov5693 2-0036 (1 pad, 1 link)
             type V4L2 subdev subtype Sensor flags 0
             device node name /dev/v4l-subdev6
	pad0: Source
		[fmt:SBGGR10_1X10/736x496@1/30 field:none]
		-> "ipu3-csi2 1":0 [ENABLED]

- entity 39: ov7251 3-0060 (1 pad, 1 link)
             type V4L2 subdev subtype Sensor flags 0
             device node name /dev/v4l-subdev7
	pad0: Source
		[fmt:SGRBG10_1X10/640x480@100/3000 field:none colorspace:srgb xfer:srgb ycbcr:601 quantization:full-range
		 crop:(0,0)/640x480]
		-> "ipu3-csi2 2":0 []

- entity 41: ov8865 3-0010 (1 pad, 1 link)
             type V4L2 subdev subtype Sensor flags 0
             device node name /dev/v4l-subdev8
	pad0: Source
		[fmt:SBGGR10_1X10/3264x2448@1/30 field:none colorspace:raw xfer:none ycbcr:601 quantization:full-range]
		-> "ipu3-csi2 0":0 [ENABLED]


/dev/media1:
Media controller API version 5.8.4

Media device information
------------------------
driver          ipu3-imgu
model           ipu3-imgu
serial          
bus info        PCI:0000:00:05.0
hw revision     0x80862015
driver version  5.8.4

Device topology
- entity 1: ipu3-imgu 0 (5 pads, 5 links)
            type V4L2 subdev subtype Unknown flags 0
            device node name /dev/v4l-subdev0
	pad0: Sink
		[fmt:FIXED/1920x1080 field:none colorspace:raw
		 crop:(0,0)/1920x1080
		 compose:(0,0)/1920x1080]
		<- "ipu3-imgu 0 input":0 []
	pad1: Sink
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		<- "ipu3-imgu 0 parameters":0 []
	pad2: Source
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		-> "ipu3-imgu 0 output":0 [ENABLED,IMMUTABLE]
	pad3: Source
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		-> "ipu3-imgu 0 viewfinder":0 []
	pad4: Source
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		-> "ipu3-imgu 0 3a stat":0 []

- entity 7: ipu3-imgu 0 input (1 pad, 1 link)
            type Node subtype V4L flags 0
            device node name /dev/video4
	pad0: Source
		-> "ipu3-imgu 0":0 []

- entity 13: ipu3-imgu 0 parameters (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video5
	pad0: Source
		-> "ipu3-imgu 0":1 []

- entity 19: ipu3-imgu 0 output (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video6
	pad0: Sink
		<- "ipu3-imgu 0":2 [ENABLED,IMMUTABLE]

- entity 25: ipu3-imgu 0 viewfinder (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video7
	pad0: Sink
		<- "ipu3-imgu 0":3 []

- entity 31: ipu3-imgu 0 3a stat (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video8
	pad0: Sink
		<- "ipu3-imgu 0":4 []

- entity 37: ipu3-imgu 1 (5 pads, 5 links)
             type V4L2 subdev subtype Unknown flags 0
             device node name /dev/v4l-subdev1
	pad0: Sink
		[fmt:FIXED/1920x1080 field:none colorspace:raw
		 crop:(0,0)/1920x1080
		 compose:(0,0)/1920x1080]
		<- "ipu3-imgu 1 input":0 []
	pad1: Sink
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		<- "ipu3-imgu 1 parameters":0 []
	pad2: Source
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		-> "ipu3-imgu 1 output":0 [ENABLED,IMMUTABLE]
	pad3: Source
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		-> "ipu3-imgu 1 viewfinder":0 []
	pad4: Source
		[fmt:FIXED/1920x1080 field:none colorspace:raw]
		-> "ipu3-imgu 1 3a stat":0 []

- entity 43: ipu3-imgu 1 input (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video9
	pad0: Source
		-> "ipu3-imgu 1":0 []

- entity 49: ipu3-imgu 1 parameters (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video10
	pad0: Source
		-> "ipu3-imgu 1":1 []

- entity 55: ipu3-imgu 1 output (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video11
	pad0: Sink
		<- "ipu3-imgu 1":2 [ENABLED,IMMUTABLE]

- entity 61: ipu3-imgu 1 viewfinder (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video12
	pad0: Sink
		<- "ipu3-imgu 1":3 []

- entity 67: ipu3-imgu 1 3a stat (1 pad, 1 link)
             type Node subtype V4L flags 0
             device node name /dev/video13
	pad0: Sink
		<- "ipu3-imgu 1":4 []


```
