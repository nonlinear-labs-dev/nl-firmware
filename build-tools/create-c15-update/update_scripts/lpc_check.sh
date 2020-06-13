#!/bin/sh

#
# last changed: 2020-06-13 KSTR
# version : 1.0
#
# ---------- check if LPC is alive after reset, using a specified number of retries  -----------
# errors will be logged and reported
#

 
LPC_RETRIES=$1
if [ -z "$1" ] ; then
  LPC_RETRIES=1
fi

function check_lpc() {
	/update/utilities/lpc reset $LPC_RETRIES
	return $?
}

check_lpc
return_code=$?
if [ $return_code -eq 0 ] ; then
	exit 0
else
	echo "E34 RT update: no response after reset" >> /update/errors.log
	exit 34
fi
