#!/bin/sh

int_trap() {
  echo
}

trigger_fetch() {
  while true; do lpc req at-test-data; sleep 0.1; done
}

echo 'Instructions:'
echo ' Load all keys with the test load until all values turned green.'
echo ' Press [CTRL-C].'
echo ' Copy the comma-separated values to spreadsheet.'
echo
read -p "Press [Enter] to start calibration readout or [CTRL+C] to abort:" dummy
echo

systemctl stop bbbb
trap "kill 0" EXIT
lpc set at-test-data on
trigger_fetch &
trap int_trap INT
lpc-read -a +ta +o
trigger_fetch &
printf "\n\n"
lpc-read -a +ta +r +q
printf "\n"
lpc set at-test-data off
echo "Note: bbbb is stopped"
