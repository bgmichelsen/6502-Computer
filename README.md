# 6502 Breadboard Computer

This is the software section for my breadboard 6502 computer. You can find the videos of the project at: [6502 Computer Playlist](https://www.youtube.com/playlist?list=PLaf-bX7Qn_0sIxjuCfPSq9A8UskGHo9l2).

The project has three sections:
- Python Scripts
- EEPROM Control Files
- Assembly Files

## Python Scripts

The most important Python script is the Binary_to_EEPROM.py file. This script takes the binary files produced from the Assembly for the 6502 and uploads it to an EEPROM module via an Arduino Mega.

## EEPROM Control

This is the INO file for the Arduino Mega. It is setup to take the binary data over serial and transfer it to an EEPROM module via the connected I/O lines.

## Assembly Files

These are simple Assembly files for the 6502. It has several test files and a Larson Scanner file. The binary files for each of the assembly files are also in this folder.

## Note:

You will need a 6502 assembler for the Assembly files. Something like [cc65](https://www.cc65.org/) should work well.
