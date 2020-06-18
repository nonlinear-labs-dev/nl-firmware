#!/bin/sh

#
# last changed: 2020-06-16 KSTR
# version : 1.1
#
# ---------- Clear Log Files -----------
# source : https://unix.stackexchange.com/questions/139513/how-to-clear-journalctl/333899#333899
#

find /var/log/journal -name "*.journal" | xargs rm; systemctl restart systemd-journald
