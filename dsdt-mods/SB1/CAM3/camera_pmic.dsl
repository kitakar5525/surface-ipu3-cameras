Scope (_SB.PCI0.I2C3)
{
    Device (SKC2)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Name (_HID, "INT3472")  // _HID: Hardware ID
        Name (_CID, "INT3472")  // _CID: Compatible ID
        Name (_DDN, "INCL-CRDD")  // _DDN: DOS Device Name
        Name (_UID, "2")  // _UID: Unique ID
        Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
        {
            Name (SBUF, ResourceTemplate ()
            {
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x004F
                    }
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x0050
                    }
            })
            Return (SBUF) /* \_SB_.PCI0.I2C3.SKC2._CRS.SBUF */
        }

        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            If (CL02)
            {
                Return (0x0F)
            }

            Return (Zero)
        }

        Method (CLDB, 0, NotSerialized)
        {
            Name (PAR, Buffer (0x20)
            {
                /* 0000 */  0x00, 0x01, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,  // ... ....
                /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // ........
            })
            Return (PAR) /* \_SB_.PCI0.I2C3.SKC2.CLDB.PAR_ */
        }

        Method (_DSM, 4, NotSerialized)  // _DSM: Device-Specific Method
        {
            If ((Arg0 == ToUUID ("79234640-9e10-4fea-a5c1-b5aa8b19756f")))
            {
                If ((Arg2 == Zero))
                {
                    If ((Arg1 == Zero))
                    {
                        Return (Buffer (One)
                        {
                                0x0F                                             // .
                        })
                    }
                }

                If ((Arg1 == Zero))
                {
                    If ((Arg2 == One))
                    {
                        Return (0x02)
                    }

                    If ((Arg2 == 0x02))
                    {
                        Return (0x01004F0C)
                    }

                    If ((Arg2 == 0x03))
                    {
                        Return (0x01005000)
                    }
                }
                Else
                {
                    ADBG ("SKC2: Revision 1 not supported")
                }
            }

            Return (Zero)
        }
    } /* Device (SKC2) */
} /* Scope (_SB.PCI0.I2C3) */
