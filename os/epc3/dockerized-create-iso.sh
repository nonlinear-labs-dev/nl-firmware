#!/bin/sh

set -e

echo "Copy base system config..."

cp -r /usr/share/archiso/configs/baseline/ /archlive
echo "arch-install-scripts
base
dosfstools
efibootmgr
erofs-utils
linux
mkinitcpio
mkinitcpio-archiso
nano
syslinux
" > /archlive/packages.x86_64

mkdir -p /archlive/airootfs/packages

add_package() {
    for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
        for PLATFORM in "x86_64" "any"; do
            fileName="$1-$2-$PLATFORM.$ENDING"
            if [ -f /packages/$fileName ]; then
                echo "add /packages/$fileName to iso image"
                cp /packages/$fileName /archlive/airootfs/packages/
                return 0
            fi
        done
    done

    return 1
}

add_packages() {
  echo "Add packages to iso..."
  
  while read -r package || [ -n "$package" ] ; do
    if [ ! -z "$package" ]; then
      name=$(echo $package | cut -f1 -d " ")
      version=$(echo $package | cut -f2 -d " ")
      add_package $name $version || return 1
    fi
  done < /src/packages.txt
  
  echo "Copy additional packages..."
  
  cp out/epc3-base.db /archlive/airootfs/packages/
  cp out/epc3-base.db.tar.zst /archlive/airootfs/packages/
  cp out/epc3-base.files /archlive/airootfs/packages/
  cp out/epc3-base.files.tar.zst /archlive/airootfs/packages/
  cp out/epc3-base-meta-1.0.0-1-any.pkg.tar.zst /archlive/airootfs/packages/
}

generate_scripts() {
  echo "Generate scripts for easy setup..."
  
  cat <<- ENDOFHERE > /archlive/airootfs/sda.sfdisk
  label: gpt
  label-id: 7D22A5F5-C3A7-4C35-B879-B58C9B422919
  device: /mnt/fedora-home/epc.img
  unit: sectors
  first-lba: 2048
  
  /dev/sda1 : start=        2048, size=     1048576, type=C12A7328-F81F-11D2-BA4B-00A0C93EC93B, uuid=37946461-1176-43E6-9F0F-5B98652B8AB9
  /dev/sda2 : start=     1050624, size=    16777216, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, uuid=274A1E65-7546-4887-86DD-771BAC588588
  /dev/sda3 : start=    17827840, size=    16777216, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, uuid=E30DE52F-B006-442E-9A4A-F332A9A0FF00
ENDOFHERE

  cat <<- ENDOFHERE > /archlive/airootfs/runme.sh
  #!/bin/sh
  
  SSD_NAME=\`lsblk -o RM,NAME | grep "^ 0" | grep -o "sd." | uniq\`
  SSD=/dev/\${SSD_NAME}
  
  while fdisk -l \${SSD} | grep "\${SSD}[0-9]"; do
   echo "\${SSD} is already partitioned"
   echo "If you are sure to know what you're doing, please type: cfdisk \${SSD}"
   echo "Delete all partitions there manually, write the partition table and retry this skript."
   echo "Here you have a shell for fixing the issue. Once you close the shell (Ctrl+D), a new attempt will be made."
   /bin/bash
  done
  
  echo "Partitioning \${SSD}:"
  
  cat /sda.sfdisk | sfdisk \${SSD}
  echo ";" | sfdisk -a \${SSD}
  
  yes | mkfs.fat \${SSD}1
  yes | mkfs.ext4 \${SSD}2
  yes | mkfs.ext4 \${SSD}3
  yes | mkfs.ext4 \${SSD}4
  
  mount \${SSD}2 /mnt
  mkdir /mnt/boot
  mount \${SSD}1 /mnt/boot
  
  echo "[epc3-base]" > /etc/pacman.conf
  echo "Server = file:///packages/" >> /etc/pacman.conf
  pacman -Sy
  
  rm -rf /var/cache/pacman/pkg
  mkdir /mnt/pkg
  ln -s /mnt/pkg /var/cache/pacman/pkg
  
  pacstrap /mnt epc3-base-meta 2>&1 | tee /log.txt
  rm -rf /mnt/pkg
ENDOFHERE
}

make_iso() {
  echo "Make iso..."
  ISONAME=$(mkarchiso -v -w /work_dir -o /out /archlive | grep -o "/out/archlinux-baseline-.*-x86_64.iso")
  mv $ISONAME /out/epc3-base.iso
}

add_packages
generate_scripts
make_iso
