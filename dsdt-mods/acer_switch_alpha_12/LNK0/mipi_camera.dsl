Scope (\_SB_.PCI0)
{
    Device (LNK0)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            If (L0EN)
            {
                Return (0x0F)
            }
            Else
            {
                Return (Zero)
            }
        }

        Method (_DEP, 0, NotSerialized)  // _DEP: Dependencies
        {
            If (L0EN)
            {
                Return (CDEP (L0CL))
            }
            Else
            {
                Return (Package (0x01)
                {
                    PCI0
                })
            }
        }

        Name (_UID, Zero)  // _UID: Unique ID
        Method (_HID, 0, NotSerialized)  // _HID: Hardware ID
        {
            Return (HCID (Zero))
        }

        Method (_CID, 0, NotSerialized)  // _CID: Compatible ID
        {
            Return (HCID (Zero))
        }

        Method (_DDN, 0, NotSerialized)  // _DDN: DOS Device Name
        {
            Name (BUF, Buffer (0x10){})
            BUF [Zero] = L0M0 /* \L0M0 */
            BUF [One] = L0M1 /* \L0M1 */
            BUF [0x02] = L0M2 /* \L0M2 */
            BUF [0x03] = L0M3 /* \L0M3 */
            BUF [0x04] = L0M4 /* \L0M4 */
            BUF [0x05] = L0M5 /* \L0M5 */
            BUF [0x06] = L0M6 /* \L0M6 */
            BUF [0x07] = L0M7 /* \L0M7 */
            BUF [0x08] = L0M8 /* \L0M8 */
            BUF [0x09] = L0M9 /* \L0M9 */
            BUF [0x0A] = L0MA /* \L0MA */
            BUF [0x0B] = L0MB /* \L0MB */
            BUF [0x0C] = L0MC /* \L0MC */
            BUF [0x0D] = L0MD /* \L0MD */
            BUF [0x0E] = L0ME /* \L0ME */
            BUF [0x0F] = L0MF /* \L0MF */
            Return (ToString (BUF, Ones))
        }

        Method (_PLD, 0, Serialized)  // _PLD: Physical Location of Device
        {
            Name (PLDB, Package (0x01)
            {
                Buffer (0x14)
                {
                    /* 0000 */  0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                    /* 0008 */  0x69, 0x0E, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,  // i.......
                    /* 0010 */  0xFF, 0xFF, 0xFF, 0xFF                           // ....
                }
            })
            CreateByteField (DerefOf (PLDB [Zero]), 0x08, BPOS)
            BPOS = L0PL /* \L0PL */
            Return (PLDB) /* \_SB_.PCI0.LNK0._PLD.PLDB */
        }

        Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
        {
            Local0 = Buffer (0x02)
                {
                        0x79, 0x00                                       // y.
                }
            If ((L0DI > Zero))
            {
                Local1 = IICB (L0A0, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > One))
            {
                Local1 = IICB (L0A1, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x02))
            {
                Local1 = IICB (L0A2, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x03))
            {
                Local1 = IICB (L0A3, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x04))
            {
                Local1 = IICB (L0A4, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x05))
            {
                Local1 = IICB (L0A5, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x06))
            {
                Local1 = IICB (L0A6, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x07))
            {
                Local1 = IICB (L0A7, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x08))
            {
                Local1 = IICB (L0A8, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x09))
            {
                Local1 = IICB (L0A9, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x0A))
            {
                Local1 = IICB (L0AA, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            If ((L0DI > 0x0B))
            {
                Local1 = IICB (L0AB, L0BS)
                ConcatenateResTemplate (Local0, Local1, Local2)
                Local0 = Local2
            }

            Return (Local0)
        }

        Method (SSDB, 0, NotSerialized)
        {
            Name (PAR, Buffer (0x6C)
            {
                /* 0000 */  0x00, 0x00, 0x69, 0x56, 0x39, 0x8A, 0xF7, 0x11,  // ..iV9...
                /* 0008 */  0xA9, 0x4E, 0x9C, 0x7D, 0x20, 0xEE, 0x0A, 0xB5,  // .N.} ...
                /* 0010 */  0xCA, 0x40, 0xA3, 0x00, 0x00, 0x00, 0x00, 0x00,  // .@......
                /* 0018 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0020 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0028 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0030 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0038 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0040 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0048 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0050 */  0x09, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,  // ........
                /* 0058 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0060 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0068 */  0x00, 0x00, 0x00, 0x00                           // ....
            })
            PAR [Zero] = L0DV /* \L0DV */
            PAR [One] = L0CV /* \L0CV */
            PAR [0x18] = CDIV /* \CDIV */
            PAR [0x1C] = L0LU /* \L0LU */
            PAR [0x1D] = L0NL /* \L0NL */
            PAR [0x4E] = L0EE /* \L0EE */
            PAR [0x4F] = L0VC /* \L0VC */
            PAR [0x52] = L0FS /* \L0FS */
            PAR [0x54] = L0DG /* \L0DG */
            CreateDWordField (PAR, 0x56, DAT)
            DAT = L0CK /* \L0CK */
            PAR [0x5A] = L0CL /* \L0CL */
            Return (PAR) /* \_SB_.PCI0.LNK0.SSDB.PAR_ */
        }

        Method (_DSM, 4, NotSerialized)  // _DSM: Device-Specific Method
        {
            If ((Arg0 == ToUUID ("822ace8f-2814-4174-a56b-5f029fe079ee")))
            {
                If ((Arg2 == Zero))
                {
                    If ((Arg1 == Zero))
                    {
                        Return (Buffer (One)
                        {
                                0x03                                             // .
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
                    Return ("CJAF510")
                }
            }

            If ((Arg0 == ToUUID ("5815c5c8-c47d-477b-9a8d-76173176414b")))
            {
                If ((Arg2 == Zero))
                {
                    If ((Arg1 == Zero))
                    {
                        Return (Buffer (One)
                        {
                                0x03                                             // .
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
                    Name (DSMB, Buffer (0x34){})
                    CreateDWordField (DSMB, Zero, I2CC)
                    CreateDWordField (DSMB, 0x04, DEV0)
                    CreateDWordField (DSMB, 0x08, DEV1)
                    CreateDWordField (DSMB, 0x0C, DEV2)
                    CreateDWordField (DSMB, 0x10, DEV3)
                    CreateDWordField (DSMB, 0x14, DEV4)
                    CreateDWordField (DSMB, 0x18, DEV5)
                    CreateDWordField (DSMB, 0x1C, DEV6)
                    CreateDWordField (DSMB, 0x20, DEV7)
                    CreateDWordField (DSMB, 0x24, DEV8)
                    CreateDWordField (DSMB, 0x28, DEV9)
                    CreateDWordField (DSMB, 0x2C, DEVA)
                    CreateDWordField (DSMB, 0x30, DEVB)
                    I2CC = L0DI /* \L0DI */
                    DEV0 = DSMI (L0D0, L0A0, Zero, L0BS)
                    DEV1 = DSMI (L0D1, L0A1, Zero, L0BS)
                    DEV2 = DSMI (L0D2, L0A2, Zero, L0BS)
                    DEV3 = DSMI (L0D3, L0A3, Zero, L0BS)
                    DEV4 = DSMI (L0D4, L0A4, Zero, L0BS)
                    DEV5 = DSMI (L0D5, L0A5, Zero, L0BS)
                    DEV6 = DSMI (L0D6, L0A6, Zero, L0BS)
                    DEV7 = DSMI (L0D7, L0A7, Zero, L0BS)
                    DEV8 = DSMI (L0D8, L0A8, Zero, L0BS)
                    DEV9 = DSMI (L0D9, L0A9, Zero, L0BS)
                    DEVA = DSMI (L0DA, L0AA, Zero, L0BS)
                    DEVB = DSMI (L0DB, L0AB, Zero, L0BS)
                    Return (DSMB) /* \_SB_.PCI0.LNK0._DSM.DSMB */
                }

                Return (Buffer (One)
                {
                        0x00                                             // .
                })
            }

            If ((Arg0 == ToUUID ("26257549-9271-4ca4-bb43-c4899d5a4881")))
            {
                If ((Arg2 == Zero))
                {
                    If ((Arg1 == Zero))
                    {
                        Return (Buffer (0x02)
                        {
                                0xFF, 0x1F                                       // ..
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
                    Return (L0DI) /* \L0DI */
                }

                If ((Arg2 == 0x02))
                {
                    If ((L0DI > Zero))
                    {
                        Return (DSMI (L0D0, L0A0, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x03))
                {
                    If ((L0DI > One))
                    {
                        Return (DSMI (L0D1, L0A1, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x04))
                {
                    If ((L0DI > 0x02))
                    {
                        Return (DSMI (L0D2, L0A2, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x05))
                {
                    If ((L0DI > 0x03))
                    {
                        Return (DSMI (L0D3, L0A3, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x06))
                {
                    If ((L0DI > 0x04))
                    {
                        Return (DSMI (L0D4, L0A4, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x07))
                {
                    If ((L0DI > 0x05))
                    {
                        Return (DSMI (L0D5, L0A5, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x08))
                {
                    If ((L0DI > 0x06))
                    {
                        Return (DSMI (L0D6, L0A6, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x08))
                {
                    If ((L0DI > 0x07))
                    {
                        Return (DSMI (L0D7, L0A7, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x09))
                {
                    If ((L0DI > 0x08))
                    {
                        Return (DSMI (L0D8, L0A8, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x0A))
                {
                    If ((L0DI > 0x09))
                    {
                        Return (DSMI (L0D9, L0A9, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x0B))
                {
                    If ((L0DI > 0x0A))
                    {
                        Return (DSMI (L0DA, L0AA, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }

                If ((Arg2 == 0x0C))
                {
                    If ((L0DI > 0x0B))
                    {
                        Return (DSMI (L0DB, L0AB, Zero, L0BS))
                    }
                    Else
                    {
                        Return (Zero)
                    }
                }
            }

            Return (Zero)
        }
    } /* Device (LNK0) */
} /* Scope (\_SB_.PCI0) */
