Scope (\_SB.PCI0.CIO2)
{
    /* Define endpoints for CIO2 device:
     - endpoint of port0 is connected to LNK0
     */

    Name (EP00, Package() {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "endpoint", 0 },
            Package () { "clock-lanes", 0 },
            Package () { "data-lanes", Package () { 1, 2 } },
            Package () { "remote-endpoint",
                Package() { \_SB.PCI0.LNK0, 0, 0 }
            },
        }
    })
}
