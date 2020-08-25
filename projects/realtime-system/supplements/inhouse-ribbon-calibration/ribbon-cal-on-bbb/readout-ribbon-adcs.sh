#!/bin/sh

function leds() {
  for i in {0..65}
  do
    printf "\x$(printf %x $i)"$1 > /dev/ribbon_led
  done
}

int_trap() {
  printf "\nDone\n"
}

systemctl stop bbbb
./lpc set sensors on
leds "\3"
trap int_trap INT
./lpc-read -a +r +i
./lpc set sensors off
leds "\0"
