#!/bin/sh

# set -x

SSH_OPTIONS="-o LogLevel=quiet -o ServerAliveInterval=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no"

function executeAsRoot() {
    if echo "sscl" | sshpass -p 'sscl' ssh $SSH_OPTIONS sscl@192.168.10.10 "sudo -S /bin/bash -c '$2'" &> /dev/null; then
        echo "$1 Yes!"
        return 0
    fi

    echo "$1 No!"
    return 1
}

function check_userland_exists() {
    executeAsRoot \
        "Userland exists?" \
        "ls /usr/local/ | grep C15"
}

function check_soundcard_driver_param() {
    executeAsRoot \
        "Soundcard driver parameter fix applied?" \
        "cat /etc/modprobe.d/snd_hda_intel.conf | grep 'snd_hda_intel bdl_pos_adj=1,1,1'"
}

function check_epc_network_exists() {
    executeAsRoot \
        "EPC Accesspoint is configured?" \
        "test -e /etc/NetworkManager/system-connections/C15.nmconnection'"
}

function check_isolcpus() {
    executeAsRoot \
        "EPC has isolated CPUS for realtime processes?" \
        "cat /proc/cmdline | grep isolcpus"
}

function check_mitigations() {
    executeAsRoot \
        "EPC has mitigations switched off?" \
        "cat /proc/cmdline | grep mitigations"
}

function check_overlay() {
    executeAsRoot \
        "OverlayFS order is correct?" \
        "mount | grep overlay | grep /nloverlay/os-overlay:/lroot"
}

function check_buffer_underruns() {
    executeAsRoot \
        "Is audio-engine running?" \
        "systemctl status audio-engine" || return 0

    echo "Now sleeping for 4 hours to look for buffer underruns..."
    DATE=$(executeAsRoot "getting time" "date +\"%y-%m-%d %T\"")
    sleep 14400

    executeAsRoot \
        "Is audio-engine running without buffer underruns?" \
        "! journalctl -u audio-engine -S \"$DATE\" | grep x-run"
}

function check_usb_stick() {
    while ! mount | grep /mnt/usb-stick > /dev/null; do
        echo "Please connect a USB stick to the BBB!"
        sleep 2
    done

    sleep 5

    rm -f /mnt/usb-stick/test-file

    executeAsRoot \
        "Write to usb stick on EPC?" \
        "echo '1' > /mnt/usb-stick/test-file"

    sleep 5

    if [ "$(cat /mnt/usb-stick/test-file)" = "1" ]; then
        echo "Could access file on usb stick that has been written by epc via sshfs? Yes!"
    else
        echo "Could access file on usb stick that has been written by epc via sshfs? No!"
    fi

   echo '1' > /mnt/usb-stick/test-file

   sleep 5

   executeAsRoot \
        "Read from usb stick on EPC?" \
        "[ \"\$(cat /mnt/usb-stick/test-file)\" = '2' ]"

}

function check_BBB_AP() {
    if executeAsRoot "Is EPC1?" "cat /proc/cpuinfo | grep -e \"i3-5010\" -e \"i3-7100\""; then
        if iw wlan0 info | grep ssid | grep -v "_BBB" > /dev/null; then
            echo "BBB Accesspoint is active and is correctly missing the _BBB suffix? Yes!"
            return 0
        fi

        echo "BBB Accesspoint is active and is correctly missing the _BBB suffix? No!"
        return 1
    fi

    if iw wlan0 info | grep ssid | grep "_BBB" > /dev/null; then
        echo "BBB Accesspoint is active and has correct _BBB suffix? Yes!"
        return 0
    else
        echo "BBB Accesspoint is active and has correct _BBB suffix? No!"
        return 1
    fi
}

function check_EPC_AP() {
    if executeAsRoot "Is EPC1?" "cat /proc/cpuinfo | grep -e \"i3-5010\" -e \"i3-7100\""; then
        if executeAsRoot "Has Wifi device?" "ip a | grep wlan"; then
            if executeAsRoot "Is AP active on epc1?" "iw wlan0 info | grep ssid"; then
                echo "EPC1 has AP disabled? No!"
                return 1
            fi
        fi
         
        echo "EPC1 has AP disabled? Yes!"
        return 0
    fi

    executeAsRoot \
        "AP name on epc>=2 has no BBB postfix?" \
        "iw wlan0 info | grep ssid | grep -v _BBB"
}

function check_iface_names() {
    executeAsRoot \
        "Ethernet device is named eth0?" \
        "ip a | grep eth0"

    executeAsRoot \
        "Wifi device is named wlan0?" \
        "ip a | grep wlan0"
}

function check_ping_timeout() {
    executeAsRoot \
        "Ping timeout workaround is installed in initramfs?" \
        "mkdir -p /mnt/sda1; mount /dev/sda1 /mnt/sda1; lsinitcpio -x /mnt/sda1/initramfs-linux-rt.img && cat hooks/nlhook | grep ping_bbb"
}


function main() {
    check_userland_exists
    check_iface_names
    check_overlay
    check_soundcard_driver_param
    check_usb_stick
    check_BBB_AP
    check_EPC_AP
    check_isolcpus
    check_mitigations
    check_ping_timeout
    check_buffer_underruns
}

main
