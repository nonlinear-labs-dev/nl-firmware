#!/bin/sh

#
# last changed: 2020-08-24 KSTR
# version : 1.0
#
# ---------- check if playcontroller is alive after reset, using a specified number of retries  -----------
# errors will be logged and reported
#

export_gpio() {
	GPIO=$1
	echo $GPIO > /sys/class/gpio/export
	sleep 0.1
	echo out > /sys/class/gpio/gpio$GPIO/direction
	sleep 0.1
}

unexport_gpio() {
	GPIO=$1
	echo $GPIO > /sys/class/gpio/unexport
	sleep 0.1
}

write_to_gpio() {
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


# set ISP high and keep it high (do NOT unexport pin
export_gpio $ISP1 2> /dev/null
export_gpio $ISP2 2> /dev/null
write_to_gpio $ISP1 1
write_to_gpio $ISP2 1

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
