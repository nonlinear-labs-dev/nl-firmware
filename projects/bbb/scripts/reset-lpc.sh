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

# we handle 2 GPIO pins for the ISP signal so that it works
# with new uniboard revisions where ISP is wired differently.
# Root cause of rewiring is that no all BBB hardware implementation
# correctly handle the GPIOs with our specific device tree and
# a new pin has been choosen that seemingly works with the affected
# BBB hardware implementation
#
# ISP GPIO pin for uniboards < 7.2 (RevD)
ISP1=48
# ISP GPIO pin for uniboards >= 7.2 (RevD)
ISP2=51

RESET=50

export_gpio $ISP1 2> /dev/null
export_gpio $ISP2 2> /dev/null
export_gpio $RESET

write_to_gpio $ISP1 1
write_to_gpio $ISP2 1
sleep 0.1
write_to_gpio $RESET 0
sleep 0.1
write_to_gpio $RESET 1
sleep 0.1

unexport_gpio $RESET
#we must not unexport ISP lines in order to keep their state
# unexport_gpio $ISP1
# unexport_gpio $ISP2
