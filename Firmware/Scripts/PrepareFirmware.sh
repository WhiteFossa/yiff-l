#!/bin/bash

hex2bin -c Yiff-HL.hex

dd if=/dev/zero ibs=1 count=98304 | tr "\000" "\377" > Yiff-HL_padded.bin

dd if=Yiff-HL.bin of=Yiff-HL_padded.bin conv=notrunc
