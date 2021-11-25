#!/bin/bash

pacman --noconfirm -Sy
pacman --noconfirm -S $1

pacman -Q | cut -f 1 -d " " > /installed-packages
pacman -Q | cut -f 1 -d " " | xargs pacman -Qi | grep "Depends On" | cut -f2 -d ":" | tr " " "\n" | grep -v '^\\w*$' | sort -u | grep -v "\.so=" > /resolved-packages

cat /installed-packages /resolved-packages | grep "^[a-zA-Z]" | grep -v "None" | sort -u | xargs pacman -Q  > out/.epc2-update-os-final-packages
