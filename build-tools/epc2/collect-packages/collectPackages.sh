#!/bin/sh

PACKAGES=$(echo @COLLECT_PACKAGES_INPUT_ARG@ | tr ' ' '\n' | xargs -i pactree -s -l {} | sort -u)
(pacman -Si $PACKAGES | grep "Version" -B1 | cut -f2,3 -d ":" | sed 's/ //g' | tr "\n" " " | sed "s/\-\-/\n/g" | sed "s/^\s*//") >> /packages.txt 2>> /errors.txt
cat /packages.txt | sed 's/\(.*\)\s$/\1/' | sort -u

for ERR in $(cat /errors.txt | grep -o "'.*'" | sed "s/'//g");do
  if ! pacman -Si @COLLECT_PACKAGES_INPUT_ARG@ | grep "Provides.*$ERR" > /dev/null; then
    echo "There is no package providing $ERR" 1>&2
    exit 1
  fi
done
