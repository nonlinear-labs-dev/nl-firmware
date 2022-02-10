#!/bin/bash

PACKAGES=$(cat /bindir/packages.txt)

apt-cache show $PACKAGES | grep "Package: \|Version: \|Architecture: \|Filename:" | while read -r X; do
  if echo $X | grep Package > /dev/null; then
    if [ "$F" != "" ]; then
      echo "${P}_${V}_${A}.deb"
    fi

    V=""
    A=""
    F=""
    P=$(echo $X | sed 's/Package: //g')
  fi
  
  if echo $X | grep Version > /dev/null; then
    V=$(echo $X | sed 's/Version: //g' | sed 's/\:/%3a/g')
  fi
  
  if echo $X | grep Architecture > /dev/null; then
    A=$(echo $X | sed 's/Architecture: //g')
  fi
  
  if echo $X | grep Filename > /dev/nullv; then
    F=$(echo $X | sed 's/Filename: //g')
  fi
done
