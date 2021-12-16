#!/usr/bin/ash

set -x

echo "I have been called from initramfs!"
rm /lib/initcpio/hooks/nlhook_v2 