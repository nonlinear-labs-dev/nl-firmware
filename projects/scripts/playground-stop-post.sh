#!/bin/sh

#
# last changed: 2018-12-14 KSTR
# version : 1.0
#
# ---------- Post processing after Playground Stop -----------
#
#


rm -f /nonlinear/playground/dirty  # remove any "Playground Dirty file

/bin/sh /nonlinear/scripts/backup-presets.sh 

exit 0
