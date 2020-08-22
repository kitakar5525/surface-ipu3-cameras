Scope (_SB.PCI0.CAM1)
{
    /* Port0 of CAM1 is connected to port1 of CIO2 device */
    Name (_DSD, Package () {
        ToUUID ("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
            Package () { "port0", "PRT0" },
        },
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
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
                Package () { 1 }
            },
            // Package () { "link-frequencies",
            //     // TODO: not sure about this value, commenting out for now
            //     Package() { ??? }
            // },
            Package () { "remote-endpoint",
                Package() { \_SB.PCI0.CIO2, 1, 0 }
            },
        }
    })
} /* Scope (_SB.PCI0.CAM1) */
