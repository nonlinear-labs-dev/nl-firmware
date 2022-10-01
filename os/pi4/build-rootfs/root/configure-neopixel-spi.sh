#!/bin/sh
dtoverlay spi0-1cs no_miso=on cs0_pin=8

# reconfigure GPIO8 (CS0) and GPIO11 (SCLK) as inputs for other uses
/usr/bin/raspi-gpio set 8 ip pd
/usr/bin/raspi-gpio set 11 ip pd
