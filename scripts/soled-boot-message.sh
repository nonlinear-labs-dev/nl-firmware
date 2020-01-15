#!/bin/sh

#
# last changed: 2018-11-15 KSTR
# version : 1.1
#
# ---------- Output a boot-message on SOLED -----------
# dual path to "text2soled" binary for backwards compatibility

if [ -x /nonlinear/text2soled/text2soled ] ;
then
	/nonlinear/text2soled/text2soled clear
	/nonlinear/text2soled/text2soled "initializing..." 5 85
else
	/nonlinear/text2soled clear
	/nonlinear/text2soled "initializing..." 5 85
fi

exit 0
