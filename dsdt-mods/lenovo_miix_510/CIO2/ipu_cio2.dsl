Scope (\_SB_.PCI0)
{
    Device (CIO2)
    {
        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            Return (0x0F)
        }

        If ((CIOE == One))
        {
            Name (_HID, "INT343E")  // _HID: Hardware ID
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Name (CBUF, ResourceTemplate ()
                {
                    Interrupt (ResourceConsumer, Level, ActiveLow, Shared, ,, _Y15)
                    {
                        0x00000010,
                    }
                    Memory32Fixed (ReadWrite,
                        0xFE400000,         // Address Base
                        0x00010000,         // Address Length
                        )
                })
                CreateDWordField (CBUF, \_SB.PCI0.CIO2._CRS._Y15._INT, CIOV)  // _INT: Interrupts
                CIOV = CIOI /* \CIOI */
                Return (CBUF) /* \_SB_.PCI0.CIO2._CRS.CBUF */
            }
        }
        Else
        {
            Name (_ADR, 0x00140003)  // _ADR: Address
        }
    }
}
