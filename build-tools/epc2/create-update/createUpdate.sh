#!/bin/sh

set -e

PACKAGES_COLLECTION=""

declare -A versions

collect_package() {
    name="$1"
    version="$2"
    
    for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
        for PLATFORM in "x86_64" "any"; do
            fileName="$name-$version-$PLATFORM.$ENDING"
            if [ -f /packages/$fileName ]; then
              PACKAGES_COLLECTION="$PACKAGES_COLLECTION /packages/$fileName"
              return 0
            fi
        done
    done
        
    echo "Could not install package $name in version $version."
    return 1
}

collect_packages() {
  for name in "${!versions[@]}"; do
    version="${versions[$name]}"
      
    EXISTING=$(pacman -Qi $name 2> /dev/null | grep Version | sed 's/Version\s*: //g')
    if [ ! "$EXISTING" = "$version" ]; then
      echo "Installing $name in version $version."
      collect_package $name $version || return 1
    fi
  done
}

get_package_with_highest_version() {
  package="$1"
  if [ ! -z "$package" ]; then
    name=$(echo $package | cut -f1 -d " ")
    version=$(echo $package | cut -f2 -d " ")
  
    if [ -z ${versions[$name]} ]; then
      versions[$name]=$version
    elif [[ ${versions[$name]} < $version ]]; then
      versions[$name]=$version
    fi
  fi
}

install_packages() {
    echo "Installing packages"
    
    while read -r package; do get_package_with_highest_version "$package"; done < /build-packages.txt
    while read -r package; do get_package_with_highest_version "$package"; done < /update-packages.txt
    
    collect_packages
    
    PACKAGES_COLLECTION=$(echo $PACKAGES_COLLECTION | tr ' ' '\n' | sort -u | tr '\n' ' ')
    pacman --noconfirm -Sy
    echo "Packages $PACKAGES_COLLECTION" 
    yes | pacman -U $PACKAGES_COLLECTION
    return 0
}

ship_kernel_update() {
  cp /kernel-update-builddir/update.tar /kernel-update.tar
}

install_overlay_backdoor() {
  cp /nlhook /lib/initcpio/hooks/nlhook
  mkdir -p /usr/local/lib/systemd/system/multi-user.target.wants
  cat <<- ENDOFHERE > /usr/local/lib/systemd/system/initramfs-backdoor.service
  [Unit]
  Before=audio-engine.service playground.service
  Description=Nonlinear-Labs initramfs backdoor installer
  
  [Service]
  Type=oneshot
  ExecStart=/usr/local/C15/scripts/installInitramfsBackdoor.sh
    
  [Install]
  WantedBy=multi-user.target
ENDOFHERE

# cat <<- ENDOFHERE > /etc/mkinitcpio.d/linux-rt.preset
#ALL_config="/etc/mkinitcpio.conf"
#ALL_kver="/boot/vmlinuz-linux-rt"
#PRESETS=('default')
#default-image="/boot/initramfs-linux-rt.img"
#ENDOFHERE

  ln -s ../initramfs-backdoor.service /usr/local/lib/systemd/system/multi-user.target.wants/initramfs-backdoor.service
  systemctl enable initramfs-backdoor 

  mkdir -p /usr/local/C15/scripts 
  cp /initramfs-hook.sh /usr/local/C15/scripts/
}

build_c15() {
  mkdir -p /bindir/build
  cd /bindir/build

  cmake -DCMAKE_INSTALL_DIR=/usr/local/C15 -DTARGET_PLATFORM=epc2 -DCMAKE_BUILD_TYPE=Release -DBUILD_EPC_SCRIPTS=On -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_WEB=Off /srcdir
  make -j8
  make install 

  mkdir -p /usr/local/C15/web
  tar -xzf /web/web.tar.gz -C /usr/local/C15/web
}

setup_network_manager() {
  cat <<- ENDOFHERE > /etc/NetworkManager/system-connections/C15.nmconnection
  [connection]
  id=C15
  uuid=61679179-6804-4197-b476-eacad1d492e4
  type=wifi
  interface-name=wlp0s20f3
  permissions=

  [wifi]
  band=bg
  channel=7
  mac-address-blacklist=
  mode=ap
  ssid=NL-C15-Unit-00000

  [wifi-security]
  key-mgmt=wpa-psk
  pairwise=ccmp;
  proto=rsn;
  psk=88888888

  [ipv4]
  address1=192.168.8.2/24,192.168.8.1
  dns-search=
  method=shared

  [ipv6]
  addr-gen-mode=stable-privacy
  dns-search=
  method=auto

  [proxy]
ENDOFHERE

  cat <<- ENDOFHERE > /etc/NetworkManager/system-connections/bbb.nmconnection
  [connection]
  id=bbb
  uuid=bbb79179-6804-4197-b476-eacad1d492e4
  type=ethernet
  interface-name=eno1
  autoconnect=true
  autoconnect-priority=100

  [ipv4]
  method=manual
  address1=192.168.10.10/24

  [ipv6]
  method=ignore
ENDOFHERE

  chmod 600 /etc/NetworkManager/system-connections/C15.nmconnection
  chmod 600 /etc/NetworkManager/system-connections/bbb.nmconnection
  systemctl enable NetworkManager
}

perform_tweaks() {
    sed -i 's/GRUB_TIMEOUT=5/GRUB_TIMEOUT=1/' /etc/default/grub
    sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT="quiet ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2"/' /etc/default/grub
    sed -i 's/^HOOKS=.*$/HOOKS=\"base udev oroot block filesystems autodetect modconf keyboard net nlhook\"/' /etc/mkinitcpio.conf
    sed -i 's/^BINARIES=.*$/BINARIES=\"tar rsync gzip lsblk udevadm\"/' /etc/mkinitcpio.conf
    sed -i 's/^MODULES=.*$/MODULES=\"e1000e\"/' /etc/mkinitcpio.conf
    sed -i "s/#governor=.*$/governor='performance'/" /etc/default/cpupower

    echo "root@192.168.10.11:/mnt/usb-stick  /mnt/usb-stick  fuse.sshfs  sshfs_sync,direct_io,cache=no,reconnect,defaults,_netdev,ServerAliveInterval=2,ServerAliveCountMax=3,StrictHostKeyChecking=off  0  0" >> /etc/fstab

    systemctl enable cpupower
    systemctl enable sshd
    useradd -m sscl
    echo 'sscl:sscl' | chpasswd
    echo "sscl ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
}

# UNUSED
cleanup_modules() {
  mkdir -p /modules-to-keep
  for file in $(cat /needed-modules.txt); do
    if [ -f $file ]; then
      mkdir -p $(dirname /modules-to-keep$file)
      mv $file /modules-to-keep$file
    fi
  done

  rm -rf /lib/modules/5.15.3.21.realtime1-1-rt

  for file in $(cat /needed-modules.txt); do
    if [ -f /modules-to-keep$file ]; then
      mkdir -p $(dirname $file)
      mv /modules-to-keep$file $file
    fi
  done

  rm -rf /modules-to-keep
}

cleanup_firmware() {
  # echo 'dyndbg="file drivers/base/firmware_loader/main.c +fmp'
  # echo 'After booting, run dmesg | grep firmware_class. Of particular interest are lines with firmware_class:fw_get_filesystem_firmware'
  mkdir -p /firmware-to-keep
  cp /usr/lib/firmware/iwlwifi-QuZ-a0-hr-b0-63.ucode /firmware-to-keep

  mkdir -p /firmware-to-keep/i915
  cp /usr/lib/firmware/i915/kbl_dmc_ver1_04.bin /firmware-to-keep/i915

  # add other firmware modules here....
  rm -rf /usr/lib/firmware
  mv /firmware-to-keep /usr/lib/firmware
}

# UNUSED
cleanup_packages() {
  for package in gcc cmake git make pkgconf ccache guile; do 
    name=$(echo $package | cut -f1 -d " ")
    yes | pacman -R "$name" 
  done 
}

# UNUSED
install_backdoor() {
  echo "no backdoor installed"
  #cp /backdoor.sh /bindir/update/backdoor.sh
  #mv /boot /bindir/update/
  #BACKDOOR_CHECKSUM=$(sha256sum /bindir/update/backdoor.sh | cut -d " " -f 1)
  #touch /bindir/update/$BACKDOOR_CHECKSUM.sign
}

package_update() {
  OUTPUT_OVERLAY_HASH=$(mount | grep -o "upperdir=.*diff," | sed 's/.*overlay2//' | sed 's/diff,/diff/' | head -n1)
  OUTPUT_OVERLAY="/host-docker$OUTPUT_OVERLAY_HASH"

  # remove unneccessary files
  rm -rf /usr/share/licenses
  # cleanup_packages
  # cleanup_modules
  cleanup_firmware

  rm -rf /bindir/update
  mkdir -p /bindir/update
  tar -C $OUTPUT_OVERLAY \
	--exclude='./srcdir' \
	--exclude='./bindir' \
  --exclude='./bindir-root' \
  --exclude='./kernel-update-builddir' \
  --exclude='./web' \
	--exclude='./host-docker' \
	--exclude='./tmp' \
	--exclude='./root' \
  --exclude='./boot' \
  --exclude='./usr/include' \
  --exclude='./var/lib/pacman' \
	-czf /bindir/update/NonLinuxOverlay.tar.gz .
  touch /bindir/update/$(sha256sum /bindir/update/NonLinuxOverlay.tar.gz | grep -o "^[^ ]*").sign
  
  # install_backdoor
  tar -C /bindir/ -cf /bindir/update.tar update
}

install_overlay_backdoor
install_packages
perform_tweaks
ship_kernel_update
build_c15
setup_network_manager
package_update
