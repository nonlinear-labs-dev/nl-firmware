#!/bin/sh

leds() {
  printf "\xFF" > /dev/ribbon_led  # reset ribbon LED driver, just in case
  for i in {0..65}
  do
    printf "\x$(printf %x $i)"$1 > /dev/ribbon_led
  done
}

int_trap() {
  printf "\nDone\n"
}

echo 'Instructions:'
echo ' Press test point at LED on both ribbons for a value pair at that point.'
echo ' After each point, press [ENTER] for a new line.'
echo ' When all 20 points have been completed, copy values into spreadsheet'
echo ' and create calibration data.'
echo ' Press [CTRL-C] to exit.'
echo
read -p "Press [Enter] to start calibration readout or [CTRL+c] to abort:" dummy
echo

systemctl stop bbbb
lpc set sensors on
leds "\3"
trap int_trap INT
lpc-read -a  +i +o
lpc set sensors off
leds "\0"
echo "Note: bbbb is stopped"
