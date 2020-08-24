Scope (\_SB.PCI0.CIO2)
{
    /* Define ports for CIO2 device:
     - endpoint of port0 is connected to LNK0
     */

    Name (_DSD, Package () {
        ToUUID ("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
            Package () { "port0", "PRT0" },
        }
    })

    Name (PRT0, Package () {
        ToUUID ("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
        Package () {
            Package () { "port", 0 }, /* csi 0 */
        },
        ToUUID ("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
        Package () {
            Package () { "endpoint0", "EP00" },
        }
    })
}
