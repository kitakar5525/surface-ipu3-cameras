Scope (_SB.PCI0.I2C3.CAMR)
{
    /* Port0 of CAMR is connected to port0 of CIO2 device */
    Name (_DSD, Package () {
        ToUUID ("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
            Package () { "port0", "PRT0" },
        },
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            // TODO: is this value OK?
            Package () { "clock-frequency", 19200000 },
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
                Package () { 1, 2, 3, 4 }
            },
            Package () { "link-frequencies",
                // TODO: is this value OK?
                Package() { 360000000 }
            },
            Package () { "remote-endpoint",
                // Package() { \_SB.PCI0.CIO2, 0, 0 }
            },
        }
    })
} /* Scope (_SB.PCI0.I2C3.CAMF) */
