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
    cat <<- ENDOFHERE > /etc/wpa_supplicant/wpa_supplicant.conf
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
wps_disabled=1
update_config=1
ENDOFHERE
  chmod 600 /etc/wpa_supplicant/wpa_supplicant.conf

  cat <<- ENDOFHERE > /etc/NetworkManager/system-connections/C15.nmconnection
[connection]
id=C15
uuid=61679179-6804-4197-b476-eacad1d492e4
type=wifi
interface-name=wlan0
permissions=

[wifi]
band=bg
channel=7
mac-address-blacklist=
mode=ap
ssid=NL-C15-Unit-00000

[wifi-security]
key-mgmt=wpa-psk
pairwise=ccmp
proto=rsn
psk=88888888
wps-method=1

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

setup_audio() {

    cat <<- ENDOFHERE > /etc/modprobe.d/snd_hda_intel.conf
# Set Soundcard Parameters
options snd_hda_intel bdl_pos_adj=1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
ENDOFHERE

    chmod 600 /etc/modprobe.d/snd_hda_intel.conf
}

perform_tweaks() {
  useradd -m sscl
  echo 'sscl:sscl' | chpasswd
  echo "sscl ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
    
  echo "en_US.UTF-8 UTF-8" > /etc/locale.gen
  locale-gen
  echo "LANG=en_US.UTF-8" > /etc/locale.conf
}

package_update() {
  OUTPUT_OVERLAY_HASH=$(mount | grep -o "upperdir=.*diff," | sed 's/.*overlay2//' | sed 's/diff,/diff/' | head -n1)
  OUTPUT_OVERLAY="/host-docker$OUTPUT_OVERLAY_HASH"

  rm -rf /bindir/update
  mkdir -p /bindir/update
  tar -C $OUTPUT_OVERLAY \
	--exclude='./srcdir' \
	--exclude='./bindir' \
  --exclude='./bindir-root' \
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

install_packages
perform_tweaks
build_c15
setup_network_manager
setup_audio
package_update
