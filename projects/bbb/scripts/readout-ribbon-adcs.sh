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
echo ' Run "readout-ribbon-adcs.sh" and collect values.'
echo ' Enter "calibration" folder and edit X-values in ribbon calibration input files (use templates).'
echo ' Run "make-cal-bin" in that folder to create LPC message.'
echo ' Copy results with scp to epc::/persistent.'
echo
read -p "Press [Enter] to start calibration readout or [CTRL+c] to abort:" dummy
echo

systemctl stop bbbb
lpc set sensors on
leds "\3"
trap int_trap INT
lpc-read -a +r +i
lpc set sensors off
leds "\0"
echo "Note: bbbb is stopped"
