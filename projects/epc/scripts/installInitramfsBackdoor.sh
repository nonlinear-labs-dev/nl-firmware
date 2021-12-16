#!/bin/sh

set -x

if [ -f /lib/initcpio/hooks/nlhook_v2 ]; then
  cp /lib/initcpio/hooks/nlhook_v2 /lib/initcpio/hooks/nlhook
  # removal of nlhook_v2 has to be done in the hook itself: build-tools/epc2/create-update/initramfs-hook.sh
  # so we olny remove the file if the hook installation did actually work properly

  mkinitcpio -p linux-rt
  reboot
fi

