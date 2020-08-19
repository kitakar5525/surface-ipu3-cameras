Scope (_SB.PCI0.I2C2)
{
    Device (SKC1)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Name (_HID, "INT3472")  // _HID: Hardware ID
        Name (_CID, "INT3472")  // _CID: Compatible ID
        Name (_DDN, "INCL-CRDD")  // _DDN: DOS Device Name
        Name (_UID, "1")  // _UID: Unique ID
        Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
        {
            Name (SBUF, ResourceTemplate ()
            {
                // Trying to add PMIC addres
                I2cSerialBusV2 (0x000c, ControllerInitiated, 0x00061A80,
                    AddressingMode7Bit, "\\_SB.PCI0.I2C3",
                    0x00, ResourceConsumer, , Exclusive,
                    )
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x0054
                    }
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x004D
                    }
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                    "\\_SB.PCI0.GPI0", 0x00, ResourceConsumer, ,
                    )
                    {   // Pin list
                        0x0016
                    }
            })
            Return (SBUF) /* \_SB_.PCI0.I2C2.SKC1._CRS.SBUF */
        }

        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            Return (0x0F)
        }

        Method (CLDB, 0, NotSerialized)
        {
            Name (PAR, Buffer (0x20)
            {
                /* 0000 */  0x00, 0x01, 0x01, 0x20, 0x00, 0x00, 0x00, 0x00,  // ... ....
                /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // ........
            })
            Return (PAR) /* \_SB_.PCI0.I2C2.SKC1.CLDB.PAR_ */
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
                                0x1F                                             // .
                        })
                    }
                }

                If ((Arg1 == Zero))
                {
                    If ((Arg2 == One))
                    {
                        Return (0x03)
                    }

                    If ((Arg2 == 0x02))
                    {
                        Return (0x0100540C)
                    }

                    If ((Arg2 == 0x03))
                    {
                        Return (0x01004D00)
                    }

                    If ((Arg2 == 0x04))
                    {
                        Return (0x0100160D)
                    }
                }
                Else
                {
                    ADBG ("SKC1: Revision 1 not supported")
                }
            }

            Return (Zero)
        }
    } /* Device (SKC1) */
} /* Scope (_SB.PCI0.I2C2) */
