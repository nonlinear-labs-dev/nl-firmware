#!/bin/sh

# reset the playcontroller and obtain and store UHID if successful
if /bin/playcontroller reset 5; then
  /bin/playcontroller req uhid64
  /bin/playcontroller-read -a -h +q +6 +o > /tmp/UHID
fi
