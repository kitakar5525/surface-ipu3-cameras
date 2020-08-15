Scope (\_SB.PCI0.CIO2)
{
    /* Define endpoints for CIO2 device:
     - endpoint of port0 is connected to CAMR
     - endpoint of port1 is connected to CAMF
     - endpoint of port2 is connected to CAM3
     */

    Name (EP00, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "endpoint", 0 },
            Package () { "clock-lanes", 0 },
            Package () { "data-lanes", Package () { 1, 2, 3, 4 } },
            Package () { "remote-endpoint",
                Package() { \_SB.PCI0.I2C3.CAMR, 0, 0 }
            },
        }
    })

    Name (EP10, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "endpoint", 0 },
            Package () { "clock-lanes", 0 },
            Package () { "data-lanes", Package () { 1, 2 } },
            Package () { "remote-endpoint",
                Package() { \_SB.PCI0.I2C2.CAMF, 0, 0 }
            },
        }
    })

    Name (EP20, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "endpoint", 0 },
            Package () { "clock-lanes", 0 },
            Package () { "data-lanes", Package () { 1 } },
            Package () { "remote-endpoint",
                Package() { \_SB.PCI0.I2C3.CAM3, 0, 0 }
            },
        }
    })
}
