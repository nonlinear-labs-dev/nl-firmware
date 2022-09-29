#!/bin/bash

PACKAGES_FILE="$1"

mkdir -p @DOWNLOAD_DIR@/os/pi4/packages
[ -f @DOWNLOAD_DIR@/os/pi4/packages/@OS_IMAGE_FILE_NAME@ ] || wget @DOWNLOAD_BASE_URL@/@OS_IMAGE_FILE_NAME@ -O @DOWNLOAD_DIR@/os/pi4/packages/@OS_IMAGE_FILE_NAME@

for FILE in $(cat $PACKAGES_FILE); do
  URL=$(echo $FILE | sed 's/%/%25/')
  [ -f @DOWNLOAD_DIR@/os/pi4/packages/$FILE ] || wget "@DOWNLOAD_BASE_URL@/$URL" -O @DOWNLOAD_DIR@/os/pi4/packages/$FILE
done
