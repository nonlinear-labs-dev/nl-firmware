#!/bin/sh

int_trap() {
  printf "\nDone\n"
}

echo 'Instructions:'
echo ' Center bender to get inverted green display.'
echo ' Check if endpoints become green.'
echo ' Check if hysteresis is within bound (still green)'
echo ' Press [CTRL-C] to exit.'
echo
read -p "Press [Enter] to start calibration readout or [CTRL+c] to abort:" dummy
echo

systemctl stop bbbb
lpc set sensors on
trap int_trap INT
lpc-read -a +b +o
lpc set sensors off
echo "Note: bbbb is stopped"
