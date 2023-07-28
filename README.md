# Agon key-events demo

This seems to do a good job of detecting keyboard events just by looking at VDP protocol data - beyond the end of SYSVAR - without any "magic".

Code compiled using the LLVM compiler:

- Can just run it by loading and running the `key.bin` program from the bin directory

Summary of function of code:

- Check `vdp_protocol` data in memory where more complete keyboard event information remains that is processed by MOS.

- This data is not "officially" accessible as part of the MOS API, but it can be examined in memory beyond the end of the SYSVARS data structure.
  
  - The location is fixed relative to the start of SYSVARS, but may change with future versions of MOS.
    
    - SYSVARS address this is obtained through mos_api call, but should not change (0xbc4d9). Likely to change in future versions of MOS
    
    - VDP protocol control (0xbc514)
    
    - VDP protocol data (0xbc51a)

The 24 bit integer at VDP control should be equal to 0x1d0001 for keyboard events

The data at VDP protocol consists of the following 3 bytes:

- Modifier (bit-mask)

- Key-code (note that this changes if the shift key is pressed)

- Down flag - events are generated for a key being pressed and a key being released

In theory the reading of the VDP control and VDP data should be done in a single "atomic" read as the values could change during the reading. In the program this is done as x2 24 bit integer reads (this is why unions are used in the program). There are other ways to do this - for example by disabling interrupts during the reads.

The program can detect multiple key presses at the same time.````
