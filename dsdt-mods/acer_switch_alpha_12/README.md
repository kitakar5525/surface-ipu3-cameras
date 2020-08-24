#### note

- `acall "\L0BS"` returns 0x2. So, I guess `DSC0` is on `I2C2`
- `acall "\C0GP"` returns 0x3.
- `DSC0` defines GPIO pins as 0x007E, 0x007F, and 0x008F, according to the output of `_CRS`