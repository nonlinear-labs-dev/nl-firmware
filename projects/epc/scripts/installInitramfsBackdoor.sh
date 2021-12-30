#!/bin/sh

set -x
set -e

mount /dev/sda2 /mnt
mount /dev/sda1 /mnt/boot

function unmount_at_exit {
  umount /mnt/boot
  umount /mnt
}

trap unmount_at_exit EXIT

DESIRED_HOOK_CHECKSUM=$(sha256sum /lib/initcpio/hooks/nlhook | cut -f1 -d ' ')
[ -f /mnt/boot/installed-hook-checksum ] && INSTALLED_HOOK_CHECKSUM=$(cat /mnt/boot/installed-hook-checksum)

if [ "$DESIRED_HOOK_CHECKSUM" = "$INSTALLED_HOOK_CHECKSUM" ]; then
  echo "desired hook is already installed"
  exit 0
fi

cp /lib/initcpio/hooks/nlhook /mnt/lib/initcpio/hooks/
sed "s/ 'fallback'//" -i /etc/mkinitcpio.d/linux-rt.preset
arch-chroot /mnt mkinitcpio -p linux-rt
echo "$DESIRED_HOOK_CHECKSUM" > /mnt/boot/installed-hook-checksum
reboot  
