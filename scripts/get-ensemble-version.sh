#!/bin/sh

#
# last changed: 2018-08-06 KSTR
# version : 1.0
#
# ---------- get Version of Reaktor Ensemble -----------
# errors are reported.
#

ping -c 1 -W 1 192.168.10.10  &>/dev/null  # 1 try and 1 sec timeout
if [ $? -ne 0 ] ; then
	echo "Error: could not connect to ePC"
	exit 1
fi

umount -f /mnt/windows  &>/dev/null  # unmount, just in case, to prevent "device busy"
mkdir -p /mnt/windows  &>/dev/null  # create the mountpoint if nonexistent
# now mount the windows-drive
mount.cifs //192.168.10.10/update /mnt/windows -o user=TEST,password=TEST  &>/dev/null
if [ $? -ne 0 ] ; then
	echo "Error: could not mount remote directory"
	exit 1
fi

ls -e /mnt/windows/Phase22Renderer.ens	# output file details (file date, especially)
if [ $? -ne 0 ] ; then
	echo "Error: No Ensemble found"
	umount -f /mnt/windows  &>/dev/null
	exit 1
fi

grep "P22 R" /mnt/windows/Phase22Renderer.ens # search the ensemble binary for the ID string
if [ $? -ne 0 ] ; then
	echo "Error: No Ensemble named P22_R* found"
	umount -f /mnt/windows  &>/dev/null
	exit 1
fi

umount -f /mnt/windows  &>/dev/null
exit 0
