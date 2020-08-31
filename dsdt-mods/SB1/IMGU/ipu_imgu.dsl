Scope (\_SB_.PCI0)
{
    Device (ISP0)
    {
        Name (_ADR, 0x00050000)  // _ADR: Address
    }
}

Scope (_SB.PCI0.ISP0)
{
    Method (_DEP, 0, NotSerialized)  // _DEP: Dependencies
    {
        If (((S0ID == One) || (OSYS >= 0x07DF)))
        {
            Return (Package (0x01)
            {
                PEPD
            })
        }
        Else
        {
            Return (Package (0x00){})
        }
    }
}
