#!/bin/sh

#
# last changed: 2020-06-13 KSTR
# version : 1.0
#
# ---------- check if playcontroller is alive after reset, using a specified number of retries  -----------
# errors will be logged and reported
#

set -x
 
PLAYCONTROLLER_RETRIES=$1
if [ -z "$1" ] ; then
  PLAYCONTROLLER_RETRIES=1
fi

check_playcontroller() {
    /update/utilities/playcontroller reset $PLAYCONTROLLER_RETRIES
	return $?
}

if ! check_playcontroller; then
	exit 0
else
	echo "E34 RT update: no response after reset" >> /update/errors.log
	exit 34
fi
