Scope (_SB.PCI0)
{
    Device (PMI0)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Name (_HID, "INT3472")  // _HID: Hardware ID
        Name (_CID, "INT3472")  // _CID: Compatible ID
        Name (_DDN, "INCL-CRDD")  // _DDN: DOS Device Name
        Name (_UID, Zero)  // _UID: Unique ID
        Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
        {
            Name (SBUF, ResourceTemplate ()
            {
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x0078
                    }
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x007B
                    }
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x0085
                    }
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x0086
                    }
            })
            Return (SBUF) /* \_SB_.PCI0.PMI0._CRS.SBUF */
        }

        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            Return (0x0F)
        }

        Method (CLDB, 0, NotSerialized)
        {
            Name (PAR, Buffer (0x20)
            {
                /* 0000 */  0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,  // ... ....
                /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // ........
            })
            Return (PAR) /* \_SB_.PCI0.PMI0.CLDB.PAR_ */
        }

        Method (_INI, 0, NotSerialized)  // _INI: Initialize
        {
            SHPO (0x02050000, One)
            SPMV (0x02050000, Zero)
            SHPO (0x02050003, One)
            SPMV (0x02050003, Zero)
            SHPO (0x0205000D, One)
            SPMV (0x0205000D, Zero)
            SHPO (0x0205000E, One)
            SPMV (0x0205000E, Zero)
        }

        Method (_DSM, 4, NotSerialized)  // _DSM: Device-Specific Method
        {
            If ((Arg0 == ToUUID ("79234640-9e10-4fea-a5c1-b5aa8b19756f")))
            {
                If ((Arg2 == One))
                {
                    Return (0x04)
                }

                If ((Arg2 == 0x02))
                {
                    Return (0x01007800)
                }

                If ((Arg2 == 0x03))
                {
                    Return (0x01007B0C)
                }

                If ((Arg2 == 0x04))
                {
                    Return (0x01008501)
                }

                If ((Arg2 == 0x05))
                {
                    Return (0x0100860B)
                }
            }

            Return (Zero)
        }
    } /* Device (PMI0) */
} /* Scope (_SB.PCI0) */
