#!/bin/sh

#
# last changed: 2018-12-13 KSTR
# version : 1.0
#
# ---------- check if ePC online, else print a dot to SOLED, lower right -----------
# dual path to "text2soled" binary for backwards compatibility

if [ -x /nonlinear/text2soled/text2soled ] ; then
	TEXT2SOLED=/nonlinear/text2soled/text2soled
else
	TEXT2SOLED=/nonlinear/text2soled
fi


while true
do
	if  ! ping -c 1 -W 1 192.168.10.10 > /dev/null  ; then
		$TEXT2SOLED "." 126 96
	fi
	if ! mount | grep "/internalstorage" > /dev/null ; then
		$TEXT2SOLED "." 126 95
	fi
	sleep 5
done
exit 0
