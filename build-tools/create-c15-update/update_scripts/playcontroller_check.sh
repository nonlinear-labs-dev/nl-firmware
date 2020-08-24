#!/bin/sh

#
# last changed: 2020-08-24 KSTR
# version : 1.0
#
# ---------- check if playcontroller is alive after reset, using a specified number of retries  -----------
# errors will be logged and reported
#

PLAYCONTROLLER_RETRIES=$1
if [ -z "$1" ] ; then
  PLAYCONTROLLER_RETRIES=1
fi

check_playcontroller() {
  /update/utilities/playcontroller reset $PLAYCONTROLLER_RETRIES
  return $?
}

check_playcontroller && exit 0

echo "E34 RT update: no response after reset" >> /update/errors.log
exit 34
