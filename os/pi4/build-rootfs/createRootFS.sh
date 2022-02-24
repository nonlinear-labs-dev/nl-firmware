#!/bin/bash

set -e

# wenn CM4, nach /boot/overlays
# ../../raspi4/Klaus/config/touch.dts
# ../../raspi4/Klaus/config/touch.dtbo

# config.txt zu config.txt.in und modifikationsschritt je nach Target einfuegen

# uebernehmen aber disablen:
# ../../raspi4/Klaus/config/configure-lcd-pwm.service
# ../../raspi4/Klaus/config/configure-lcd-pwm.sh

# ../../raspi4/Klaus/config/max-cpu.service

# ../../raspi4/Klaus/config/configure-neopixel-spi.service
# ../../raspi4/Klaus/config/configure-neopixel-spi.sh

# ../../raspi4/Klaus/config/backlight.sh

# TODO: init_resize entfernen und wenn noetig waehrend des build prozess resizen


unpack_image() {
  cp /current-binary-dir/raspios-original.img /raspios.img
}

tweak_boot_partition() {

  mount -o loop -o offset=4194304 -o sizelimit=268435456 -t vfat /raspios.img /mnt
    
  [[ "$PI4_Model" =~ "CM4" ]] && mv /boot-files/* /mnt
  
  echo -n "console=serial0,115200 "                         > /mnt/cmdline.txt
  echo -n "console=tty1 "                                   >> /mnt/cmdline.txt
  echo -n "root=PARTUUID=6c74a671-02 "                      >> /mnt/cmdline.txt
  echo -n "rootfstype=ext4 "                                >> /mnt/cmdline.txt
  echo -n "fsck.repair=yes "                                >> /mnt/cmdline.txt
  echo -n "rootwait "                                       >> /mnt/cmdline.txt
  echo -n "quiet "                                          >> /mnt/cmdline.txt
  echo -n "splash "                                         >> /mnt/cmdline.txt
  echo -n "fsck.mode=skip "                                 >> /mnt/cmdline.txt
  echo -n "noswap "                                         >> /mnt/cmdline.txt
  echo -n "systemd.restore_state=0 rfkill.default_state=1 " >> /mnt/cmdline.txt
  
  echo -n "elevator=deadline "                              >> /mnt/cmdline.txt
  echo -n "fastboot "                                       >> /mnt/cmdline.txt
  echo -n "logo.nologo "                                    >> /mnt/cmdline.txt
  
  echo -n "isolcpus=0,2 "                                   >> /mnt/cmdline.txt
      
  [[ "$PI4_FEATURES" =~ "NO_CURSOR" ]] && echo -n "vt.global_cursor_default=0 " >> /mnt/cmdline.txt
  [[ "$PI4_FEATURES" =~ "NO_X" ]] && echo -n "start_x=0 " >> /mnt/config.txt
  
  touch /mnt/ssh
  umount /mnt
}

disable_service() {
  NAME="$1"
  chroot /mnt rm -f /etc/systemd/system/multi-user.target.wants/$NAME
  chroot /mnt ln -s /dev/null /etc/systemd/system/multi-user.target.wants/$NAME 
}

enable_service() {
  NAME="$1"
  chroot /mnt rm -f /etc/systemd/system/multi-user.target.wants/$NAME
  chroot /mnt ln -s /usr/lib/systemd/system/$NAME /etc/systemd/system/multi-user.target.wants/$NAME 
}

tweak_root_partition() {
  mount -o loop -o offset=272629760 -t ext4 /raspios.img ./mnt
  
  mkdir -p /mnt/packages
  cp /downloads/os/pi4/packages/*.deb /mnt/packages
  
  PACKAGEFILES=$(cat /pi4-binary-dir/collect-packages/package-files.txt | while read -r FILE; do 
    FILE=$(echo $FILE | sed 's/%/%25/')
    [ "$FILE" != "" ] && echo -n "/packages/$FILE "
  done)
  
  # filter out already installed packages
  while read -r ALREADY; do
    P=$(echo $ALREADY | cut -d' ' -f2 | sed 's/\:armhf//g')
    V=$(echo $ALREADY | cut -d' ' -f3 | sed 's/\:/%3a/g' | sed 's/%/%25/g')
    V2=$(echo $ALREADY | cut -d' ' -f3 | sed 's/\:/%3a/g' | sed 's/\+[^ ]*//g' | sed 's/%/%25/g')
    A=$(echo $ALREADY | cut -d' ' -f4)
    F1=$(printf "/packages/%s_%s_%s.deb" $P $V $A)
    F2=$(printf "/packages/%s_%s[^ ]*_%s.deb" $P $V $A)
    F3=$(printf "/packages/%s_%s[^ ]*_%s.deb" $P $V2 $A)
    PACKAGEFILES=$(echo $PACKAGEFILES | sed "s|$F1||g" | sed "s|$F2||g" | sed "s|$F3||g")
  done <<< $(chroot /mnt dpkg -l | grep "^ii" | sed 's/[ ]* / /g')

  echo "Now installing packages: $PACKAGEFILES"

  FIXED_PACKAGFILES="";

  for p in $PACKAGEFILES; do
    if [ ! -f $p ]; then
        c=$(echo $p | sed 's/%25/%/g')
        FIXED_PACKAGFILES="$FIXED_PACKAGFILES $c"
    else
        FIXED_PACKAGFILES="$FIXED_PACKAGFILES $p"
    fi
  done

  chroot /mnt dpkg -i $FIXED_PACKAGFILES
  
  echo "hostname"                       > /mnt/etc/dhcpcd.conf
  echo "clientid"                       >> /mnt/etc/dhcpcd.conf
  echo "persistent"                     >> /mnt/etc/dhcpcd.conf
  echo "option rapid_commit"            >> /mnt/etc/dhcpcd.conf
  echo "option interface_mtu"           >> /mnt/etc/dhcpcd.conf
  echo "require dhcp_server_identifier" >> /mnt/etc/dhcpcd.conf
  echo "slaac private"                  >> /mnt/etc/dhcpcd.conf
  
  if [[ "$PI4_ETH" =~ "STATIC:" ]]; then
    IP=$(echo "$PI4_ETH" | cut -d ':' -f2)
    ROUTERS=$(echo "$PI4_ETH" | cut -d ':' -f3)
    DNS=$(echo "$PI4_ETH" | cut -d ':' -f4)
    
    echo "profile static_eth0"                                                >> /mnt/etc/dhcpcd.conf
    echo "static ip_address=$IP"                                              >> /mnt/etc/dhcpcd.conf
    echo "static routers=$ROUTERS"                                            >> /mnt/etc/dhcpcd.conf
    echo "static domain_name_servers=$DNS"                                    >> /mnt/etc/dhcpcd.conf
    echo ""                                                                   >> /mnt/etc/dhcpcd.conf
    echo "interface eth0"                                                     >> /mnt/etc/dhcpcd.conf
    echo "fallback static_eth0"                                               >> /mnt/etc/dhcpcd.conf
  elif [ "$PI4_ETH" = "DHCP" ]; then
    echo "option domain_name_servers, domain_name, domain_search, host_name"  >> /mnt/etc/dhcpcd.conf
    echo "option classless_static_routes"                                     >> /mnt/etc/dhcpcd.conf
  fi
  
  if [[ "$PI4_WIFI" =~ "STATIC:" ]]; then
    IP=$(echo "$PI4_WIFI" | cut -d ':' -f2)
    ROUTERS=$(echo "$PI4_WIFI" | cut -d ':' -f3)
    DNS=$(echo "$PI4_WIFI" | cut -d ':' -f4)
    
    echo "profile static_wifi"              >> /mnt/etc/dhcpcd.conf
    echo "static ip_address=$IP"            >> /mnt/etc/dhcpcd.conf
    echo "static routers=$ROUTERS"          >> /mnt/etc/dhcpcd.conf
    echo "static domain_name_servers=$DNS"  >> /mnt/etc/dhcpcd.conf
    echo ""                                 >> /mnt/etc/dhcpcd.conf
    echo "interface wlan0"                  >> /mnt/etc/dhcpcd.conf
    echo "fallback static_wifi"             >> /mnt/etc/dhcpcd.conf
  elif [[ "$PI4_WIFI" =~ "CLIENT:" ]]; then
    SSID=$(echo "$PI4_WIFI" | cut -d ':' -f2)
    PSK=$(echo "$PI4_WIFI" | cut -d ':' -f3)
    echo "ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev"  > /mnt/etc/wpa_supplicant/wpa_supplicant.conf
    echo "update_config=1"                                          >> /mnt/etc/wpa_supplicant/wpa_supplicant.conf
    echo "country=DE"                                               >> /mnt/etc/wpa_supplicant/wpa_supplicant.conf
    echo "network={"                                                >> /mnt/etc/wpa_supplicant/wpa_supplicant.conf
    echo "  ssid=\"$SSID\""                                         >> /mnt/etc/wpa_supplicant/wpa_supplicant.conf
    echo "  psk=\"$PSK\""                                           >> /mnt/etc/wpa_supplicant/wpa_supplicant.conf
    echo "}"                                                        >> /mnt/etc/wpa_supplicant/wpa_supplicant.conf
  elif [[ "$PI4_WIFI" =~ "AP:" ]]; then
    SSID=$(echo "$PI4_WIFI" | cut -d ':' -f2)
    PHRASE=$(echo "$PI4_WIFI" | cut -d ':' -f3)
    echo "interface=wlan0"        > /mnt/etc/hostapd/hostapd.conf
    echo "ssid=$SSID"             >> /mnt/etc/hostapd/hostapd.conf
    echo "hw_mode=g"              >> /mnt/etc/hostapd/hostapd.conf
    echo "channel=7"              >> /mnt/etc/hostapd/hostapd.conf
    echo "macaddr_acl="           >> /mnt/etc/hostapd/hostapd.conf
    echo "auth_algs=1"            >> /mnt/etc/hostapd/hostapd.conf
    echo "ignore_broadcast_ssid=" >> /mnt/etc/hostapd/hostapd.conf
    echo "wpa=2"                  >> /mnt/etc/hostapd/hostapd.conf
    echo "wpa_passphrase=$PHRASE" >> /mnt/etc/hostapd/hostapd.conf
    echo "wpa_key_mgmt=WPA-PSK"   >> /mnt/etc/hostapd/hostapd.conf
    echo "wpa_pairwise=TKIP"      >> /mnt/etc/hostapd/hostapd.conf
    echo "rsn_pairwise=CCMP"      >> /mnt/etc/hostapd/hostapd.conf 
    
    echo "interface wlan0"              >> /mnt/etc/dhcpcd.conf
    echo "ip_address=192.168.8.2/24"    >> /mnt/etc/dhcpcd.conf
    echo "nohook wpa_supplicant"        >> /mnt/etc/dhcpcd.conf
    
    enable_service hostapd.service
        
    sed "s/eth0/wlan0/g" -i /mnt/etc/udhcpd.conf
  fi
  
  if [[ "$PI4_FEATURES" =~ "LCD_PWM" ]]; then
    cp /root-files/configure-lcd-pwm.sh /usr/bin/configure-lcd-pwm.sh
    cp /root-files/configure-lcd-pwm.service /mnt/usr/lib/systemd/system/
    enable_service configure-lcd-pwm.service
  fi
  
  if [[ "$PI4_FEATURES" =~ "NEOPIXEL_SPI" ]]; then
    cp /root-files/configure-neopixel-spi.sh /usr/bin/configure-lcd-pwm.sh
    cp /root-files/configure-neopixel-spi.service /mnt/usr/lib/systemd/system/
    enable_service configure-neopixel-spi.service
  fi
  
  if [[ "$PI4_FEATURES" =~ "MAX_CPU" ]]; then
    cp /root-files/max-cpu.service /mnt/usr/lib/systemd/system/
    enable_service max-cpu.service 
  fi
  
  cp /root-files/backlight.sh /mnt/usr/bin/
    
  disable_service dphys-swapfile.service
  disable_service cpufrequtils.service
  disable_service loadcpufreq.service

  # add init foo here
  echo "cpufreq-set -g performance" >> /mnt/etc/rc.local

  # why?
  # chroot /mnt systemctl disable alsa-restore.service

  #TODO
  #/boot aus /etc/fstab rausnehmen
 
  rm -rf /mnt/packages
  
  chroot /mnt bash -c "useradd -m sscl"
  chroot /mnt bash -c "echo 'sscl:sscl' | chpasswd"
  echo "sscl ALL=(ALL) NOPASSWD:ALL" >> /mnt/etc/sudoers
  
  echo "xserver-command=X -s 0 -p 0 -dpms" >> /mnt/etc/lightdm/lightdm.conf
  
  chroot /mnt /bin/sh -c "DEBIAN_FRONTEND=noninteractive apt -y remove build-essential g++ g++-10 gcc emacsen-common gdb vlc sound-theme-freedesktop pulseaudio"
  chroot /mnt /bin/sh -c "DEBIAN_FRONTEND=noninteractive apt -y autoremove"
  
  rm /mnt/etc/xdg/autostart/piwiz.desktop
  rm /mnt/etc/xdg/autostart/pulseaudio.desktop
  rm /mnt/etc/xdg/autostart/pprompt.desktop
  rm /mnt/etc/xdg/autostart/print-applet.desktop
  
  umount /mnt
}

main() {
  unpack_image
  tweak_boot_partition
  tweak_root_partition
  cp /raspios.img /current-binary-dir/
}

main
