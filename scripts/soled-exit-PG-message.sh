#!/bin/sh

#
# last changed: 2018-11-15 KSTR
# version : 1.1
#
# ---------- Stop PG and output an exit message on SOLED -----------
# dual path to "text2soled" binary for backwards compatibility

killall -s SIGTERM playground
sleep 0.5

if [ -x /nonlinear/text2soled/text2soled ] ;
then
	/nonlinear/text2soled/text2soled clear
	/nonlinear/text2soled/text2soled "exiting UI..." 5 85
else
	/nonlinear/text2soled clear
	/nonlinear/text2soled "exiting UI..." 5 85
fi

exit 0
