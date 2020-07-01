### get_acpi_ssdb_sensor_data

#### building module

```bash
make
```

#### loading module

```bash
$ sudo rmmod ov5693
sudo rmmod ov8865
sudo rmmod ov7251
sleep 1
sudo insmod ov5693.ko
sudo insmod ov8865.ko
sudo insmod ov7251.ko
```

#### result on SB1

```bash
ov5693 i2c-INT33BE:00: sensor acpi data: link 1, lanes 2, mclk 0, flash 2
ov5693 i2c-INT33BE:00: sensor_data.romtype: 1
ov5693 i2c-INT33BE:00: sensor_data.vcmtype: 0
ov5693 i2c-INT33BE:00: sensor_data.privacyled: 1
ov5693 i2c-INT33BE:00: sensor_data.degree: 1
ov5693 i2c-INT33BE:00: sensor_data.mipilinkdefined: 1
ov5693 i2c-INT33BE:00: sensor_data.mclkspeed: 19200000
ov5693 i2c-INT33BE:00: sensor_data.controllogicid: 1

ov8865 i2c-INT347A:00: sensor acpi data: link 0, lanes 4, mclk 0, flash 2
ov8865 i2c-INT347A:00: sensor_data.romtype: 1
ov8865 i2c-INT347A:00: sensor_data.vcmtype: 4
ov8865 i2c-INT347A:00: sensor_data.privacyled: 1
ov8865 i2c-INT347A:00: sensor_data.degree: 1
ov8865 i2c-INT347A:00: sensor_data.mipilinkdefined: 1
ov8865 i2c-INT347A:00: sensor_data.mclkspeed: 19200000
ov8865 i2c-INT347A:00: sensor_data.controllogicid: 0

ov7251 i2c-INT347E:00: sensor acpi data: link 2, lanes 1, mclk 0, flash 2
ov7251 i2c-INT347E:00: sensor_data.romtype: 0
ov7251 i2c-INT347E:00: sensor_data.vcmtype: 0
ov7251 i2c-INT347E:00: sensor_data.privacyled: 0
ov7251 i2c-INT347E:00: sensor_data.degree: 0
ov7251 i2c-INT347E:00: sensor_data.mipilinkdefined: 1
ov7251 i2c-INT347E:00: sensor_data.mclkspeed: 19200000
ov7251 i2c-INT347E:00: sensor_data.controllogicid: 2
```
