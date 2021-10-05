#!/bin/sh

if /bin/playcontroller reset 5; then
  /bin/playcontroller req uhid64
  /bin/playcontroller-read -a -h +q +6 +o > /tmp/UHID
  /usr/C15/text2soled/text2soled multitext "Starting the C15...@SC0" "#`/bin/cat /tmp/UHID`@SC2" "Welcome,@BC2" "those who love to play keys!@BC4"
fi
