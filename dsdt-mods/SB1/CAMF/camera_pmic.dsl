Scope (_SB.PCI0.I2C2)
{
    Device (SKC1)
    {
        Name (_ADR, Zero)  // _ADR: Address
        Name (_HID, "INT3472")  // _HID: Hardware ID
        Name (_CID, "INT3472")  // _CID: Compatible ID
        Name (_DDN, "INCL-CRDD")  // _DDN: DOS Device Name
        Name (_UID, "1")  // _UID: Unique ID

        OperationRegion (GPOP, GeneralPurposeIo, 0, 0x2)
        Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
        {
            Name (SBUF, ResourceTemplate ()
            {
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

        Field (\_SB.PCI0.I2C2.SKC1.GPOP, ByteAcc, NoLock, Preserve)
        {
            Connection
            (
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000,
                    IoRestrictionInputOnly, "\\_SB.PCI0.GPI0",
                    0x00, ResourceConsumer,,)
                {
                    0x0054
                }
            ),
            GP54, 1,
            Connection
            (
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000,
                    IoRestrictionInputOnly, "\\_SB.PCI0.GPI0",
                    0x00, ResourceConsumer,,)
                {
                    0x004D
                }
            ),
            GP4D, 1,
            Connection
            (
                GpioIo (Exclusive, PullDefault, 0x0000, 0x0000,
                    IoRestrictionInputOnly, "\\_SB.PCI0.GPI0",
                    0x00, ResourceConsumer,,)
                {
                    0x0016
                }
            ),
            GP16, 1,
        }

        /* Set or clear GP54 GPIO */
        Method (CG54, 1, Serialized)
        {
            GP54 = Arg0
        }

        /* Set or clear GP4D GPIO */
        Method (CG4D, 1, Serialized)
        {
            GP4D = Arg0
        }

        /* Set or clear GP16 GPIO */
        Method (CG16, 1, Serialized)
        {
            GP16 = Arg0
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

        /* Marks the availability of all the operation regions */
        Name (AVBL, Zero)
        Name (AVGP, Zero)
        Name (AVB0, Zero)
        Name (AVB1, Zero)
        Name (AVB2, Zero)
        Name (AVB3, Zero)
        Method (_REG, 2, NotSerialized)
        {
            If (LEqual (Arg0, 0x08))
            {
                /* Marks the availability of GeneralPurposeIO
                 * 0x08: opregion space for GeneralPurposeIO
                 */
                Store (Arg1, AVGP)
            }
            If (LEqual (Arg0, 0xB0))
            {
                /* Marks the availability of
                 * TI_PMIC_POWER_OPREGION_ID */
                Store (Arg1, AVB0)
            }
            If (LEqual (Arg0, 0xB1))
            {
                /* Marks the availability of
                 * TI_PMIC_VR_VAL_OPREGION_ID */
                Store (Arg1, AVB1)
            }
            If (LEqual (Arg0, 0xB2))
            {
                /* Marks the availability of
                 * TI_PMIC_CLK_OPREGION_ID */
                Store (Arg1, AVB2)
            }
            If (LEqual (Arg0, 0xB3))
            {
                /* Marks the availability of
                 * TI_PMIC_CLK_FREQ_OPREGION_ID */
                Store (Arg1, AVB3)
            }
            If (LAnd (AVGP, LAnd (LAnd (AVB0, AVB1),
                             LAnd(AVB2, AVB3))))
            {
                /* Marks the availability of all opregions */
                Store (1, AVBL)
            }
            Else
            {
                Store (0, AVBL)
            }
        }

        /* PMIC operation regions */
        /* 0xB0: TI_PMIC_POWER_OPREGION_ID
         * VSIO: Sensor IO LDO output
         * VCMC: VCM LDO output
         * VAX1: Auxiliary LDO1 output
         * VAX2: Auxiliary LDO2 output
         * VACT: Analog LDO output
         * VDCT: Core buck output
         */
        OperationRegion (PWR1, 0xB0, Zero, 0x0100)
        Field (PWR1, DWordAcc, NoLock, Preserve)
        {
            VSIO, 32,
            VCMC, 32,
            VAX1, 32,
            VAX2, 32,
            VACT, 32,
            VDCT, 32,
        }

        /* 0xB1: TI_PMIC_VR_VAL_OPREGION_ID
         * SIOV: VSIO VR voltage value
         * IOVA: VIO VR voltage value
         * VCMV: VCM VR voltage value
         * AX1V: Auxiliary LDO1 VR voltage value
         * AX2V: Auxiliary LDO2 VR voltage value
         * ACVA: Analog LDO VR voltage
         * DCVA: Core buck VR volatage
         */
        OperationRegion (PWR2, 0xB1, Zero, 0x0100)
        Field (PWR2, DWordAcc, NoLock, Preserve)
        {
            SIOV, 32,
            IOVA, 32,
            VCMV, 32,
            AX1V, 32,
            AX2V, 32,
            ACVA, 32,
            DCVA, 32,
        }

        /* 0xB2: TI_PMIC_CLK_OPREGION_ID
         * PCTL: PLL control register
         * PCT2: PLL control 2 register
         * CFG1: Clock configuration 1 register
         * CFG2: Clock configuration 2 register
         */
        OperationRegion (CLKC, 0xB2, Zero, 0x0100)
        Field (CLKC, DWordAcc, NoLock, Preserve)
        {
            PCTL, 32,
            PCT2, 32,
            CFG1, 32,
            CFG2, 32,
        }

        /* 0xB3: TI_PMIC_CLK_FREQ_OPREGION_ID
         * PDV2: PLL output divider for HCLK_B
         * BODI: PLL output divider for boost clock
         * BUDI: PLL output divider for buck clock
         * PSWR: PLL reference clock setting
         * XTDV: Reference crystal divider
         * PLDV: PLL feedback divider
         * PODV: PLL output divider for HCLK_A
         */
        OperationRegion (CLKF, 0xB3, Zero, 0x0100)
        Field (CLKF, DWordAcc, NoLock, Preserve)
        {
            PDV2, 32,
            BODI, 32,
            BUDI, 32,
            PSWR, 32,
            XTDV, 32,
            PLDV, 32,
            PODV, 32,
        }

        /* CLE0 and CLE1 are used to determine if both the clocks
        are enabled or disabled. */
        Mutex (MUTC, 0)
        Name (CLE0, 0)
        Name (CLE1, 0)
        Method (CLKE, 0, Serialized) {
            /* save Acquire result so we can check for
            Mutex acquired */
            Store (Acquire (MUTC, 1000), Local0)
            /* check for Mutex acquired */
            If (LEqual (Local0, Zero)) {
            /* Enable clocks only when a sensor is turned on and
            both the clocks are disabled */
                If (LNot (LOr (CLE0, CLE1))) {
                    /* Set boost clock divider */
                    BODI = 3
                    /* Set buck clock divider */
                    BUDI = 2
                    /* Set the PLL_REF_CLK cyles */
                    PSWR = 19
                    /* Set the reference crystal divider */
                    XTDV = 170
                    /* Set PLL feedback divider */
                    PLDV = 32
                    /* Set PLL output divider for HCLK_A */
                    PODV = 1
                    /* Set PLL output divider for HCLK_B */
                    PDV2 = 1
                    /* Enable clocks for both the sensors
                     * CFG1: output selection for HCLK_A and
                     * HCLK_B.
                     * CFG2: set drive strength for HCLK_A
                     * and HCLK_B.
                     */
                    CFG2 = 5
                    CFG1 = 10
                    /* Enable PLL output, crystal oscillator
                     * input capacitance control and set
                     * Xtal oscillator as clock source.
                     */
                    PCTL = 209
                    Sleep(1)
                }
                Release (MUTC)
            }
        }

        /* Reference count for VSIO */
        Name (VSIC, 0)
        Method (DOVD, 1, Serialized) {
            /* Turn off VSIO */
            If (LEqual (Arg0, Zero)) {
                /* Decrement only if VSIC > 0 */
                if (LGreater (VSIC, 0)) {
                    Decrement (VSIC)
                    If (LEqual (VSIC, Zero)) {
                        VSIO = 0
                        Sleep(1)
                        // PMOF()
                    }
                }
            } ElseIf (LEqual (Arg0, 1)) {
                /* Increment only if VSIC < 4 */
                If (LLess (VSIC, 4)) {
                    /* Turn on VSIO */
                    If (LEqual (VSIC, Zero)) {
                        // PMON()
                        VSIO = 3

                        if (LNotEqual (IOVA, 52)) {
                            /* Set VSIO value as
                            1.8006 V */
                            IOVA = 52
                        }
                        if (LNotEqual (SIOV, 52)) {
                            /* Set VSIO value as
                            1.8006 V */
                            SIOV = 52
                        }
                        Sleep(3)
                    }
                    Increment (VSIC)
                }
            }
        }

        /* Clocks need to be disabled only if both the sensors are
        turned off */
        Method (CLKD, 0, Serialized) {
            /* save Acquire result so we can check for
            Mutex acquired */
            Store (Acquire (MUTC, 1000), Local0)
            /* check for Mutex acquired */
            If (LEqual (Local0, Zero)) {
                If (LNot (LOr (CLE0, CLE1))) {
                    BODI = 0
                    BUDI = 0
                    PSWR = 0
                    XTDV = 0
                    PLDV = 0
                    PODV = 0
                    PDV2 = 0
                    /* Disable clocks for both the
                    sensors */
                    CFG2 = 0
                    CFG1 = 0
                    PCTL = 0
                }
                Release (MUTC)
            }
        }

        /* Power resource methods for CAM1 */
        PowerResource (OVFI, 0, 0) {
            Name (STA, 0)
            Method (_ON, 0, Serialized) {
                /* TODO: Read Voltage and Sleep values from Sensor Obj */
                If (LEqual (AVBL, 1)) {
                    If (LEqual (STA, 0)) {
                        /* Enable VSIO regulator +
                        daisy chain */
                        DOVD(1)

                        /* Set VAUX2 as 1.8006 V */
                        AX2V = 52
                        VAX2 = 1 /* Enable VAUX2 */

                        // \_SB.PCI0.I2C2.SKC1.CG4F(1)

                        /*
                         * Wait for DOVDD and AVDD
                         * to settle.
                         */
                        Sleep(1)

                        /* Set VAUX1 as 1.2132V */
                        AX1V = 19
                        VAX1 = 1 /* Enable VAUX1 */

                        /* Wait for VDD to settle. */
                        Sleep(1)

                        \_SB.PCI0.I2C2.SKC1.CLKE()
                        CLE1 = 1

                        // \_SB.PCI0.I2C2.SKC1.CG50(1)
                        /*
                         * Ensure 10 ms between
                         * power-up and streamon.
                         */
                        Sleep(10)
                        STA = 1
                    }
                }
            }

            Method (_OFF, 0, Serialized) {
                If (LEqual (AVBL, 1)) {
                    If (LEqual (STA, 1)) {
                        Sleep(2)
                        CLE1 = 0
                        \_SB.PCI0.I2C2.SKC1.CLKD()
                        Sleep(2)
                        // \_SB.PCI0.I2C2.SKC1.CG50(0)
                        Sleep(3)
                        VAX1 = 0
                        Sleep(1)
                        // \_SB.PCI0.I2C2.SKC1.CG4F(0)
                        Sleep(1)
                        VAX2 = 0
                        Sleep(1)
                        DOVD(0)
                    }
                    STA = 0
                }
            }

            Method (_STA, 0, NotSerialized) {
                Return (STA)
            }
        }
    } /* Device (SKC1) */
} /* Scope (_SB.PCI0.I2C2) */
