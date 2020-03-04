#!/bin/sh

#
# last changed: 2018-12-14 KSTR
# version : 1.0
#
# ---------- backup C15  presets -----------
#
#

# Backup Presets
if [ -d /internalstorage/preset-manager ] ; then # nominal location "/internalstorage/preset-manager", either a dir or mounted flash
	PRESET_SOURCE_DIR=/internalstorage/preset-manager
elif [ -d /preset-manager ] ; then # alternate (non-flash) location "/internalstorage/preset-manager"
	PRESET_SOURCE_DIR=/preset-manager
else # nothing to backup, exit gracefully
	exit 0
fi

rm -rf  /preset-manager.bak  # delete any previous backup data


if ! cp -ar  $PRESET_SOURCE_DIR  /preset-manager.bak
then
	rm -rf  /preset-manager.bak  # delete any previous backup data
else
	echo "Preset manager data auto-backup time:" > /preset-manager.bak/info.txt
	date >> /preset-manager.bak/info.txt
fi
exit 0
