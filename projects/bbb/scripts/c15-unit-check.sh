#!/bin/sh
# set -x

SSH_OPTIONS="-o LogLevel=quiet -o ServerAliveInterval=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no"
EPC_IP="192.168.10.10"
LOG_FILE=""

function executeAsRoot() {
    if echo "sscl" | sshpass -p 'sscl' ssh ${SSH_OPTIONS} sscl@$EPC_IP "sudo -S /bin/bash -c '$2'" &> /dev/null; then
        echo "$1 Yes!" | tee -a $LOG_FILE
        return 0
    fi

    echo "$1 No!" | tee -a $LOG_FILE
    return 1
}

function getInfoAsRoot() {
    echo "sscl" | sshpass -p 'sscl' ssh ${SSH_OPTIONS} sscl@$EPC_IP "sudo -S /bin/bash -c '$1'"
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
    if ! executeAsRoot "Is EPC1?" "cat /proc/cpuinfo | grep -e \"i3-5010\" -e \"i3-7100\""; then
        if executeAsRoot "Has Wifi device?" "ip a | grep wlan"; then
            if executeAsRoot "Is AP active on epc?" "nmcli con show --active | grep C15"; then
                echo "EPC has AP enabled? Yes!"
                return 0
            fi
        fi
        echo "EPC has AP enabled? No!"
        return 1
    fi
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

function check_date() {
    printf "\nePC - date: $(getInfoAsRoot 'date "+%F %T"')\n" | tee -a $LOG_FILE \
        && printf "BBB - date: $(date "+%F %T")\n" | tee -a $LOG_FILE
    printf "\n" | tee -a $LOG_FILE
}

function check_ro_mnts() {
    printf "ePC - RO mounts:\n$(getInfoAsRoot 'mount | grep "(ro"')\n" | tee -a $LOG_FILE \
        && printf "BBB - RO mounts:\n$(mount | grep "(ro")\n" | tee -a $LOG_FILE
    printf "\n" | tee -a $LOG_FILE
}

function get_journal_dmesg() {
    rm /tmp/journal_epc.log /tmp/dmesg_epc.log /tmp/journal_bbb.log /tmp/dmesg_bbb.log

    getInfoAsRoot 'journalctl -b -p "emerg".."warning"' >> /tmp/journal_epc.log \
    && getInfoAsRoot 'dmesg --level=err,warn' >> /tmp/dmesg_epc.log \
    && printf "Got journal and dmesg from ePC: Yes!\n" | tee -a $LOG_FILE \
    || printf "Got journal and dmesg from ePC: No!\n" | tee -a $LOG_FILE

    journalctl -b -p "emerg".."warning" >> /tmp/journal_bbb.log \
    && dmesg --level=err,warn >> /tmp/dmesg_bbb.log \
    && printf "Got journal and dmesg from BBB: Yes!\n" | tee -a $LOG_FILE \
    || printf "Got journal and dmesg from BBB: No!\n" | tee -a $LOG_FILE

    printf "\n" | tee -a $LOG_FILE
    return 0
}

function get_partition_info() {
    printf "ePC - lsblk:\n$(getInfoAsRoot 'lsblk')\n" | tee -a $LOG_FILE \
    && printf "ePC - mount:\n$(getInfoAsRoot 'mount | grep /dev/sda')\n" | tee -a $LOG_FILE

    printf "BBB - lsblk:\n$(lsblk)\n" | tee -a $LOG_FILE \
    && printf "BBB - mount:\n$(mount | grep /dev/mmc)\n" | tee -a $LOG_FILE

    printf "\n" | tee -a $LOG_FILE
    return 0
}

function check_persistent_files() {
    executeAsRoot \
        "Calibration files?" \
        "ls /persistent/calibration/*.cal"
    executeAsRoot \
        "Calibration .bin file?" \
        "ls /persistent/calibration.bin"
    executeAsRoot \
        "Calibration .ini file?" \
        "ls /persistent/calibration.ini"
    executeAsRoot \
        "Settings.xml file?" \
        "ls /persistent/settings.xml"

    printf "\n" | tee -a $LOG_FILE
}

function check_alsa_settings() {
    executeAsRoot \
        "Alsamixer set to 100%?" \
        "amixer -D hw:PCH sget 'Master' | grep 100%"
    printf "\n" | tee -a $LOG_FILE
}

function get_network_settings_bbb() {
    cat /etc/network/interfaces | tee -a $LOG_FILE \
    && printf "\n" | tee -a $LOG_FILE \
    && cat /etc/hostapd.conf | tee -a $LOG_FILE
    printf "\n" | tee -a $LOG_FILE
}

function get_update_script_vers() {
    VERS=$(cat /usr/C15/scripts/install-update.sh | grep version)
    printf "Update script ${VERS} \n" | tee -a $LOG_FILE
    printf "\n" | tee -a $LOG_FILE
}

function check_for_present_tools() {
    for i in sshpass rsync socat thttpd playcontroller mke2fs; do
        if command -V "$i" &> /dev/null; then
            printf "$i installed: Yes\n" | tee -a $LOG_FILE
        else
            printf "$i installed: No\n" | tee -a $LOG_FILE
        fi
    done

    if [[ -x /usr/C15/text2soled/text2soled ]]; then
        printf "text2soled installed: Yes\n" | tee -a $LOG_FILE
    else
        printf "text2soled installed: No\n" | tee -a $LOG_FILE
    fi

    if [[ -x /nonlinear/scripts/mxli ]]; then
        printf "mxli installed: Yes\n" | tee -a $LOG_FILE
    else
        printf "mxli installed: No\n" | tee -a $LOG_FILE
    fi

    printf "\n" | tee -a $LOG_FILE
    return 0
}

function check_lpc() {
    systemctl stop bbbb \
    && lpc reset && printf "LPC reset: Yes\n" | tee -a $LOG_FILE \
    || printf "LPC reset: No\n" | tee -a $LOG_FILE

    printf "LPC version: $(lpc req sw-version; lpc-read +q -h) \n" | tee -a $LOG_FILE \
    || printf "Can not get LPC version!\n" | tee -a $LOG_FILE

    printf "\n" | tee -a $LOG_FILE
    systemctl restart bbbb
    return 0
}

function run_epc_stress_test() {

    while ( ! getInfoAsRoot 'grep "/dev/sdb " /proc/mounts' ); do
        executeAsRoot "Created /tmp/usb?" "mkdir /tmp/usb"
        executeAsRoot "Unmounted USB Stick?" "umount /dev/sdb"
        executeAsRoot "Mounted USB Stick?" "mount /dev/sdb /tmp/usb" \
        || read -p "Can not find USB stick! Try again!"
    done

    if ( ! getInfoAsRoot "[ -x /tmp/usb/Diagnosis_Tools/Benchmarks/Prime95_Linux_19/mprime ]" ); then
        read -p "Wrong USB stick! Try again!" && exit 1
    fi

    TEST_DURATION=$1
    CHECK_FREQ=$2
    printf "Will run stress test on ePC for ${TEST_DURATION}s\n" | tee -a $LOG_FILE
    getInfoAsRoot "/tmp/usb/Diagnosis_Tools/Benchmarks/Prime95_Linux_19/mprime -t &> /dev/null &"
    executeAsRoot "Benchmark started?" "pidof mprime"

    for COUNTER in $(seq 0 $CHECK_FREQ $TEST_DURATION); do
        TEMP_CORE_0=$(getInfoAsRoot 'sensors | grep "Core 0:" | cut -d"+" -f2 | cut -d"." -f1')
        [ $TEMP_CORE_0 -ge 85 ] && printf "Core 0 critical: $TEMP_CORE_0\n" | tee -a $LOG_FILE && break

        TEMP_CORE_1=$(getInfoAsRoot 'sensors | grep "Core 1:" | cut -d"+" -f2 | cut -d"." -f1')
        [ $TEMP_CORE_1 -ge 85 ] && printf "Core 1 critical: $TEMP_CORE_1\n" | tee -a $LOG_FILE && break

        echo "CORE 0 = $TEMP_CORE_0; CORE 1 = $TEMP_CORE_1; Duration: $COUNTER/$TEST_DURATION" | tee -a $LOG_FILE
        sleep $CHECK_FREQ
    done

    executeAsRoot "Stopped benchmark?" 'kill $(pidof mprime)'
    executeAsRoot "Deleted benchmark files?" 'rm /tmp/usb/Diagnosis_Tools/Benchmarks/Prime95_Linux_19/prime.txt \
        /tmp/usb/Diagnosis_Tools/Benchmarks/Prime95_Linux_19/local.txt  \
        /tmp/usb/Diagnosis_Tools/Benchmarks/Prime95_Linux_19/results.txt'
    executeAsRoot "Unmounted USB?" "umount /dev/sdb"
    return 0
}

function check_buffer_underruns() {
    TEST_DURATION=$1
    CHECK_FREQ=$2

    executeAsRoot \
        "Is audio-engine running?" \
        "systemctl status audio-engine" || return 0

    printf "Will run buffer underruns test on ePC for ${TEST_DURATION}s\n" | tee -a $LOG_FILE

    DATE=$(getInfoAsRoot 'date "+%y-%m-%d %T"')

    for COUNTER in $(seq 0 $CHECK_FREQ $TEST_DURATION); do
        if ( $(getInfoAsRoot "journalctl -u audio-engine -S \"${DATE}\" | grep -q x-run") ); then
            printf "Bufferunderun detected! " | tee -a $LOG_FILE \
            && getInfoAsRoot 'date "+%y-%m-%d %T"' | tee -a $LOG_FILE && return 1
        fi
        sleep $CHECK_FREQ;
    done
    return 0
}

function usage() {
    echo "Runs a system check. I'd advise to select a singular option!"
    echo
    echo "Syntax: ./foo.sh [ -h ] [ -b DURATION,FACTOR ] [ -s DURATION,FACTOR] [ -g ]"
    echo
    echo "options:"
    echo "-h          Usage output."
    echo "-b          Run buffer underruns test. DURATION of the test (Default 14400s), FACTOR will determine the output ratio (Default every 60s)."
    echo "-s          Run stress test. DURATION of the test (Default 1200s), FACTOR will determine the output ratio (Default every 60s)."
    echo "            Critical Temperature is set to 85C"
    echo "-g          Run general check."
    echo
}

function general_check() {
        printf "Will run general C15 check on ePC and BBB\n" | tee -a $LOG_FILE
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

        check_date
        check_ro_mnts
        get_journal_dmesg
        get_partition_info

        check_persistent_files
        check_alsa_settings

        get_network_settings_bbb
        get_update_script_vers
        check_for_present_tools
        check_lpc
}

while getopts ":hgbs" option; do
    case $option in
        b)
            set -f; OLDIFS=$IFS; IFS=','; NUMARR=($2); set +f; IFS=$OLDIFS;
            [ -z "${NUMARR[0]}" ] && DURATION=14400 || DURATION="${NUMARR[0]}"
            [ -z "${NUMARR[1]}" ] && FACTOR=60 || FACTOR="${NUMARR[1]}"
            LOG_FILE="/tmp/buffer_test.log"
            rm $LOG_FILE; touch $LOG_FILE
            check_buffer_underruns ${DURATION} ${FACTOR};;
        s)
            set -f; OLDIFS=$IFS; IFS=','; NUMARR=($2); set +f; IFS=$OLDIFS;
            LOG_FILE="/tmp/stress_test.log"
            rm $LOG_FILE; touch $LOG_FILE
            [ -z "${NUMARR[0]}" ] && DURATION=1200 || DURATION="${NUMARR[0]}"
            [ -z "${NUMARR[1]}" ] && FACTOR=60 || FACTOR="${NUMARR[1]}"
            run_epc_stress_test ${DURATION} ${FACTOR};;
        g)
            LOG_FILE="/tmp/general_check.log"
            rm $LOG_FILE; touch $LOG_FILE
            general_check;;
        h | \?)
            usage
            exit;;
        esac
done

[ $OPTIND -eq 1 ] && usage
