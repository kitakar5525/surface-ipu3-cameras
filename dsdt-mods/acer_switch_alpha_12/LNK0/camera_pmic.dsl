Scope (\_SB_.PCI0)
{
    Device (DSC0)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Name (_HID, "INT3472")  // _HID: Hardware ID
        Name (_CID, "INT3472")  // _CID: Compatible ID
        Name (_DDN, "PMIC-CRDG")  // _DDN: DOS Device Name
        Name (_UID, Zero)  // _UID: Unique ID
        Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
        {
            Local0 = Buffer (0x02)
                {
                        0x79, 0x00                                       // y.
                }
            If ((C0GP > Zero))
            {
                Local1 = PINR (C0P0, C0G0)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((C0GP > One))
            {
                Local1 = PINR (C0P1, C0G1)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((C0GP > 0x02))
            {
                Local1 = PINR (C0P2, C0G2)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((C0GP > 0x03))
            {
                Local1 = PINR (C0P3, C0G3)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            Return (Local0)
        }

        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            If (CL00)
            {
                If ((C0TP == One))
                {
                    Return (0x0F)
                }
            }

            Return (Zero)
        }

        Method (CLDB, 0, NotSerialized)
        {
            Name (PAR, Buffer (0x20)
            {
                /* 0000 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // ........
            })
            PAR [One] = C0TP /* \C0TP */
            PAR [0x03] = C0CV /* \C0CV */
            Return (PAR) /* \_SB_.PCI0.DSC0.CLDB.PAR_ */
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
                                0x3F                                             // ?
                        })
                    }
                    Else
                    {
                        Return (Buffer (One)
                        {
                                0x01                                             // .
                        })
                    }
                }

                If ((Arg2 == One))
                {
                    Return (C0GP) /* \C0GP */
                }

                If ((Arg2 == 0x02))
                {
                    Return (GPPI (C0F0, ((0x18 * C0G0) + C0P0), C0I0, C0A0))
                }

                If ((Arg2 == 0x03))
                {
                    Return (GPPI (C0F1, ((0x18 * C0G1) + C0P1), C0I1, C0A1))
                }

                If ((Arg2 == 0x04))
                {
                    Return (GPPI (C0F2, ((0x18 * C0G2) + C0P2), C0I2, C0A2))
                }

                If ((Arg2 == 0x05))
                {
                    Return (GPPI (C0F3, ((0x18 * C0G3) + C0P3), C0I3, C0A3))
                }

                Return (Buffer (One)
                {
                        0x00                                             // .
                })
            }
        }

        Method (_INI, 0, NotSerialized)  // _INI: Initialize
        {
            SHPO (0x02050006, One)
            SPMV (0x02050006, Zero)
            SHPO (0x02050007, One)
            SPMV (0x02050007, Zero)
            SHPO (0x02050017, One)
            SPMV (0x02050017, Zero)
        }
    } /* Device (DSC0) */
} /* Scope (\_SB_.PCI0) */
