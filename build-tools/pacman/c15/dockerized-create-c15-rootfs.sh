#!/bin/sh

set -e
set -x

### Installs c15 package and all its dependencies into a folder and packs the result into a tar.gz
### This results in a rootfs that can be booted and run the full NL software stack.

setup_package_database() { # Create a database containing c15 package and all its dependencies
  set -e
  mkdir -p /nl
  repo-add -q /nl/nl.db.tar.gz /packages/*.pkg.tar.zst /packages/*.pkg.tar.xz /out/c15-1.0.0-1-any.pkg.tar.zst
  rsync -av --progress /packages/* /nl/ --exclude node_modules
  cp /out/c15-1.0.0-1-any.pkg.tar.zst /nl/
  echo "[nl]" > /etc/pacman.conf
  echo "Server = file:///nl/" >> /etc/pacman.conf
  echo "SigLevel = Optional TrustAll" >> /etc/pacman.conf
  set +e
}

create_rootfs() { # Create clean folder and install c15 package and all its dependencies
  DIR=/p
  mkdir -p $DIR
  pacman -Sy
  pacstrap $DIR c15
}

tweak_rootfs() { # Tweak the resulting rootfs
  sed -i "s/#governor=.*$/governor='performance'/" $DIR/etc/default/cpupower
  echo "root@192.168.10.11:/mnt/usb-stick  /mnt/usb-stick  fuse.sshfs  sshfs_sync,direct_io,cache=no,reconnect,defaults,_netdev,ServerAliveInterval=2,ServerAliveCountMax=3,StrictHostKeyChecking=off  0  0" > $DIR/etc/fstab
  echo "options snd_hda_intel power_save=0" >> $DIR/etc/modprobe.d/snd_hda_intel.conf
  
  echo "[Coredump]" > $DIR//etc/systemd/coredump.conf
  echo "Storage=none" >> $DIR//etc/systemd/coredump.conf
  echo "ProcessSizeMax=0" >> $DIR//etc/systemd/coredump.conf

  arch-chroot $DIR ssh-keygen -A
  arch-chroot $DIR systemctl enable cpupower
  arch-chroot $DIR systemctl enable sshd
  arch-chroot $DIR systemctl enable NetworkManager 
  arch-chroot $DIR useradd -m sscl
  echo 'sscl:sscl' | arch-chroot $DIR chpasswd
  echo 'sscl ALL=(ALL) NOPASSWD:ALL' >> $DIR/etc/sudoers
  arch-chroot $DIR chown root:root /
  arch-chroot $DIR chmod 755 /
}

cleanup_rootfs() { # Remove unused stuff to keep the resulting tar small
  rm -rf $DIR/var/cache
  rm -rf $DIR/var/lib/pacman
  rm -rf $DIR/usr/include
  rm -rf $DIR/usr/share/doc
  rm -rf $DIR/usr/share/man
  rm -rf $DIR/usr/share/locale
  rm -rf $DIR/usr/share/i18n/locales
  rm -rf $DIR/etc/pacman.d
  rm -rf $DIR/usr/lib/libgo.*
  find $DIR/lib/firmware | grep -v "iwlwifi-QuZ" | xargs -I {} rm -rf {}
}

create_package() { # create the tarball
  cd $DIR
  tar -cf /out/c15-rootfs.tar .
  rm -rf /out/c15-rootfs.tar.xz
  xz -9 -z /out/c15-rootfs.tar
}
 

set -x
set -e
setup_package_database
create_rootfs
tweak_rootfs
cleanup_rootfs
create_package
