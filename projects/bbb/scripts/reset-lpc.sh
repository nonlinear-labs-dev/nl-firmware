#!/bin/sh
function export_gpio {
	GPIO=$1
	echo $GPIO > /sys/class/gpio/export
	sleep 0.1
	echo out > /sys/class/gpio/gpio$GPIO/direction
	sleep 0.1
}
function unexport_gpio {
	GPIO=$1
	echo $GPIO > /sys/class/gpio/unexport
	sleep 0.1
}
function write_to_gpio {
	GPIO=$1
	VALUE=$2
	echo $VALUE > /sys/class/gpio/gpio$GPIO/value
	sleep 0.1
}
RESET=50
export_gpio $RESET
write_to_gpio $RESET 0
sleep 0.1
write_to_gpio $RESET 1
sleep 0.1
unexport_gpio $RESET
