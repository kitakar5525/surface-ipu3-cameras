Scope (_SB.PCI0.I2C2)
{
    Device (CAMF)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Name (_HID, "INT33BE" /* Camera Sensor OV5693 */)  // _HID: Hardware ID
        Name (_CID, "INT33BE" /* Camera Sensor OV5693 */)  // _CID: Compatible ID
        Name (_DDN, "OV5693-CRDD")  // _DDN: DOS Device Name
        Name (_SUB, "MSHW0070")  // _SUB: Subsystem ID
        Name (_UID, "0")  // _UID: Unique ID
        Name (_DEP, Package (0x01)  // _DEP: Dependencies
        {
            SKC1
        })
        Name (_PR0, Package (0x01)  // _PR0: Power Resources for D0
        {
            CAMP
        })
        Name (_PLD, Package (0x01)  // _PLD: Physical Location of Device
        {
            ToPLD (
                PLD_Revision           = 0x2,
                PLD_IgnoreColor        = 0x1,
                PLD_Red                = 0x0,
                PLD_Green              = 0x0,
                PLD_Blue               = 0x0,
                PLD_Width              = 0x0,
                PLD_Height             = 0x0,
                PLD_UserVisible        = 0x1,
                PLD_Dock               = 0x0,
                PLD_Lid                = 0x0,
                PLD_Panel              = "FRONT",
                PLD_VerticalPosition   = "CENTER",
                PLD_HorizontalPosition = "RIGHT",
                PLD_Shape              = "VERTICALRECTANGLE",
                PLD_GroupOrientation   = 0x0,
                PLD_GroupToken         = 0x0,
                PLD_GroupPosition      = 0x0,
                PLD_Bay                = 0x0,
                PLD_Ejectable          = 0x1,
                PLD_EjectRequired      = 0x1,
                PLD_CabinetNumber      = 0x0,
                PLD_CardCageNumber     = 0x0,
                PLD_Reference          = 0x0,
                PLD_Rotation           = 0x0,
                PLD_Order              = 0x0,
                PLD_VerticalOffset     = 0xFFFF,
                PLD_HorizontalOffset   = 0xFFFF)

        })
        Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
        {
            Name (SBUF, ResourceTemplate ()
            {
                I2cSerialBusV2 (0x0036, ControllerInitiated, 0x00061A80,
                    AddressingMode7Bit, "\\_SB.PCI0.I2C2",
                    0x00, ResourceConsumer, , Exclusive,
                    )
            })
            Return (SBUF) /* \_SB_.PCI0.I2C2.CAMF._CRS.SBUF */
        }

        Method (_STA, 0, NotSerialized)  // _STA: Status
        {
            Return (0x0F)
        }

        Method (SSDB, 0, NotSerialized)
        {
            Name (PAR, Buffer (0x6C)
            {
                /* 0000 */  0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // . ......
                /* 0008 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0010 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0018 */  0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00,  // ........
                /* 0020 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0028 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0030 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0038 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0040 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0048 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,  // ........
                /* 0050 */  0x09, 0x00, 0x02, 0x01, 0x01, 0x01, 0x00, 0xF8,  // ........
                /* 0058 */  0x24, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // $.......
                /* 0060 */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // ........
                /* 0068 */  0x00, 0x00, 0x00, 0x00                           // ....
            })
            Return (PAR) /* \_SB_.PCI0.I2C2.CAMF.SSDB.PAR_ */
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
                }

                If ((Arg1 == Zero))
                {
                    If ((Arg2 == One))
                    {
                        Return ("MSHW0070")
                    }
                }
                Else
                {
                    ADBG ("CAMF: Revision 1 not supported")
                }
            }

            If ((Arg0 == ToUUID ("26257549-9271-4ca4-bb43-c4899d5a4881")))
            {
                If ((Arg2 == Zero))
                {
                    If ((Arg1 == Zero))
                    {
                        Return (Buffer (One)
                        {
                                0x07                                             // .
                        })
                    }
                }

                If ((Arg1 == Zero))
                {
                    If ((Arg2 == One))
                    {
                        Return (One)
                    }

                    If ((Arg2 == 0x02))
                    {
                        Return (0x02003600)
                    }
                }
                Else
                {
                    ADBG ("CAMF: Revision 1 not supported")
                }
            }

            Return (Zero)
        }
    } /* Device (CAMF) */
} /* Scope (_SB.PCI0.I2C2) */
