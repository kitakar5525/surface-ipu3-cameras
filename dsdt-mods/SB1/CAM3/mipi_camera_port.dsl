Scope (_SB.PCI0.I2C3.CAM3)
{
    /* Port0 of CAM3 is connected to port2 of CIO2 device */
    Name (_DSD, Package () {
        ToUUID ("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
            Package () { "port0", "PRT0" },
        },
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            /* "external clock must be 24MHz, allow 1% tolerance"
                https://github.com/torvalds/linux/blob/d8924c0d76aaa52e4811b5c64115d9a7f36cc73a/drivers/media/i2c/ov7251.c#L1302 */
            Package () { "clock-frequency", 24000000 },
        }
    })

    Name (PRT0, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "port", 0 },
        },
        ToUUID ("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
            Package () { "endpoint0", "EP00" },
        }
    })

    Name (EP00, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "endpoint", 0 },
            Package () { "clock-lanes", 0 },
            Package () { "data-lanes",
                Package () { 1 }
            },
            /* this value is from this line:
                https://github.com/torvalds/linux/blob/d8924c0d76aaa52e4811b5c64115d9a7f36cc73a/drivers/media/i2c/ov7251.c#L522
                This value won't be read from here but hardcoded in that line.
                However, let's define also here just in case. */
            Package () { "link-frequencies",
                Package() { 240000000 }
            },
            Package () { "remote-endpoint",
                Package() { \_SB.PCI0.CIO2, 2, 0 }
            },
        }
    })
} /* Scope (_SB.PCI0.I2C3.CAM3) */
