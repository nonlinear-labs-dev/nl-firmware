#!/bin/sh

# version :     1.0
#
# ----------- EPC2 (NUC10i3 & NUC11i3) Fixes  ---------
# - prevent renaming network deivces
# - add igc module to initcpio
#
# Procedure: will be copied to the ePC und /tmp after update and executed

set -x

epc_fix() {
  mkdir -p /tmp/sda2
  mount /dev/sda2 /tmp/sda2
  mount /dev/sda1 /tmp/sda2/boot

  if ! cat /proc/cmdline | grep "net.ifnames=0 biosdevname=0" > /dev/null; then
    arch-chroot /tmp/sda2 /bin/bash -c "sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT=\"quiet net.ifnames=0 biosdevname=0 ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2\"/g' /etc/default/grub"
    arch-chroot /tmp/sda2 /bin/bash -c "grub-mkconfig -o /boot/grub/grub.cfg" \
      || printf "E48 ePC2 fix: setting Kernel Params failed" >> /tmp/fix_error.log && ((fix_errors++))
  fi

  if ! cat /tmp/sda2/etc/mkinitcpio.conf | grep "MODULES=\"e1000e igc\"" > /dev/null; then
    arch-chroot /tmp/sda2 /bin/bash -c "sed -i 's/^MODULES=.*$/MODULES=\"e1000e igc\"/g' /etc/mkinitcpio.conf"
    arch-chroot /tmp/sda2 /bin/bash -c "mkinitcpio -p linux-rt" \
      || printf "E48 ePC2 fix: adding ethernet module igc" >> /tmp/fix_error.log && ((fix_errors++))
  fi

  umount /tmp/sda2/boot
  umount /tmp/sda2
  if [ $fix_errors -gt 0 ]; then
       return 48
  fi
  return 0
}


main() {
    epc_fix
    return $?
}

main
