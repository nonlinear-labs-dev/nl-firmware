#!/bin/sh

set -e

PACKAGES_COLLECTION=""

collect_package() {
    name="$1"

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

install_packages() {
  echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
  locale-gen

  while read -r package; do
    if [ ! -z "$package" ]; then
      name=$(echo $package | cut -f1 -d " ")
      version=$(echo $package | cut -f2 -d " ")

      EXISTING=$(pacman -Qi $name 2> /dev/null | grep Version | sed 's/Version\s*: //g')
      if [ ! "$EXISTING" = "$version" ]; then
        echo "Installing $name in version $version."
        collect_package $name $version || return 1
       fi
    fi

  done < /bindir-root/build-tools/epc2/collect-packages/update-packages.txt
  
  pacman --noconfirm -U /packages/linux-firmware-20210919.d526e04-1-any.pkg.tar.zst \
                        /packages/linux-rt-5.15.3.21.realtime1-1-x86_64.pkg.tar.zst \
                        /packages/glibc-2.33-5-x86_64.pkg.tar.zst \
                        /packages/kmod-29-1-x86_64.pkg.tar.zst

  PACKAGES_COLLECTION=$(echo $PACKAGES_COLLECTION | tr ' ' '\n' | sort -u | tr '\n' ' ')
  pacman --noconfirm -U $PACKAGES_COLLECTION
  return 0
}

ship_kernel_update() {
  cp /kernel-update-builddir/update.tar /kernel-update.tar
}

install_overlay_backdoor() {
  cp /nlhook_v2 /lib/initcpio/hooks/nlhook_v2
  mkdir -p /usr/local/lib/systemd/system/multi-user.target.wants
  cat <<- ENDOFHERE > /usr/local/lib/systemd/system/initramfs-backdoor.service
  [Unit]
  Description=Nonlinear-Labs initramfs backdoor installer
  After=multi-user.target
  
  [Service]
  Type=oneshot
  ExecStart=/usr/local/C15/scripts/installInitramfsBackdoor.sh
    
  [Install]
  WantedBy=multi-user.target
ENDOFHERE

  cat <<- ENDOFHERE > /etc/mkinitcpio.d/linux-rt.preset
ALL_config="/etc/mkinitcpio.conf"
ALL_kver="/boot/vmlinuz-linux-rt"
PRESETS=('default')
default-image="/boot/initramfs-linux-rt.img"
ENDOFHERE

  ln -s ../initramfs-backdoor.service /usr/local/lib/systemd/system/multi-user.target.wants/initramfs-backdoor.service
  mkdir -p /usr/local/C15/scripts 
  cp /initramfs-hook.sh /usr/local/C15/scripts/
}

build_c15() {
  mkdir -p /bindir/build
  cd /bindir/build

  cmake _DCMAKE_INSTALL_DIR=/usr/local/C15 -DTARGET_PLATFORM=epc2 -DCMAKE_BUILD_TYPE=Release -DBUILD_EPC_SCRIPTS=On -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_WEB=Off /srcdir
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

  chmod 600 /etc/NetworkManager/system-connections/C15.nmconnection
  systemctl enable NetworkManager
}

package_update() {
  OUTPUT_OVERLAY_HASH=$(mount | grep -o "upperdir=.*diff," | sed 's/.*overlay2//' | sed 's/diff,/diff/' | head -n1)
  OUTPUT_OVERLAY="/host-docker$OUTPUT_OVERLAY_HASH"

  # remove unneccessary files
  mkdir /firmware-to-keep
  cp /usr/lib/firmware/iwlwifi* /firmware-to-keep
  rm -rf /usr/lib/firmware
  mv /firmware-to-keep /usr/lib/firmware
  rm -rf /usr/share/licenses

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
	--exclude='./etc/passwd' \
	--exclude='./etc/group' \
	--exclude='./etc/shadow' \
	-vczf /bindir/update/NonLinuxOverlay.tar.gz .
  touch /bindir/update/$(sha256sum /bindir/update/NonLinuxOverlay.tar.gz | grep -o "^[^ ]*").sign
  
  # cp /backdoor.sh /bindir/update/backdoor.sh
  # BACKDOOR_CHECKSUM=$(sha256sum /bindir/update/backdoor.sh | cut -d " " -f 1)
  # touch /bindir/update/$BACKDOOR_CHECKSUM.sign
  tar -C /bindir/ -cf /bindir/update.tar update
}

install_packages
ship_kernel_update
install_overlay_backdoor
build_c15
setup_network_manager
package_update
