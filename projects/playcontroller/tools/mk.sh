#!/bin/sh
gcc -Os -s -Wall -Wextra -I../../ -I../ src/$1.c src/linuxgpio.c src/process-read-msgs.c ../../shared/playcontroller/EHC-pedal-presets.c ../../shared/playcontroller/lpc_lib.c -o $1
