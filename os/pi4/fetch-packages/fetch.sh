#!/bin/sh

cd /fetched
apt-get download $(cat /collect-packages/packages.txt)

while read -r line; do 
  if [ "$line" != "" ]; then
    if ! [ -f /fetched/$line ]; then
      echo "file $line missing!"
      exit 1
    fi
  fi
done < /collect-packages/package-files.txt

