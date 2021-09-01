#!/bin/sh

# This function should be run, once the BBB has booted from the SD Card.
# it shall format the eMMC, rsync with the rootfs present on the SD Card,
# deploy the MLO and u-boot.img, tidy up and be done!
# After this the eMMC should be happy booting from eMMC without any SD Card Dependancy! 
# boooyah!
