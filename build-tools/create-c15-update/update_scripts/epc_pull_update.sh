#!/bin/sh

# version : 2.0
#
# ----------- install a ePC Updates from USB-Stick ---------
#
# This scripts will serve the update.tar via thttpd on the BBB and
# restart the ePC, which will evoke the Update

# timeout is so long because the initramFS will potentialy be rebuild which takes quite a while

set -x

EPC_IP=$1
TIMEOUT=300

report_and_quit(){
    printf "$1" >> /update/errors.log
    exit $2
}

executeAsRoot() {
    echo "sscl" | /update/utilities/sshpass -p 'sscl' ssh -o ServerAliveInterval=1 -o ConnectionAttempts=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no sscl@$EPC_IP "sudo -S /bin/bash -c '$1' "
    return $?
}

wait4playground() {
    for COUNTER in $(seq 1 $TIMEOUT); do
        echo "awaiting reboot ... $COUNTER/$TIMEOUT"
        sleep 1
        executeAsRoot "systemctl status playground" && return 0
    done
    return 1
}

check_preconditions(){
    [ -z "$EPC_IP" ] && report_and_quit "E81: Usage: $EPC_IP <IP-of-ePC> wrong ..." "81"
    ping -c1 $EPC_IP 1>&2 > /dev/null || report_and_quit "E82: Can't ping ePC on $EPC_IP ..." "82"
    executeAsRoot "exit" || report_and_quit "E83: Can't logon to ePC OS ..." "83"
}

add_ping_test_to_nlhook() {
  executeAsRoot "mount /dev/sda2 /mnt"
  executeAsRoot "mount /dev/sda1 /mnt/boot/"

  if ! executeAsRoot "cat /mnt/usr/lib/initcpio/hooks/nlhook | grep ping_bbb"; then
    LINE_NUMBER=$(executeAsRoot "cat /mnt/usr/lib/initcpio/hooks/nlhook | grep -n \"try_update_from_network() {\" | cut -f1 -d:")
    NEXT_LINE=$(( ${LINE_NUMBER} + 1 ))
    executeAsRoot "sed -i \"${NEXT_LINE}i       ping_bbb\" /mnt/usr/lib/initcpio/hooks/nlhook"
    executeAsRoot "sed -i \"${LINE_NUMBER}i ping_bbb() { for R in \$\(seq 20\)\; do ping -c1 -W1 192.168.10.11 \&\& break\; done \; }\" /mnt/usr/lib/initcpio/hooks/nlhook"
    executeAsRoot "mount /dev/sda1 /mnt/boot/"
    executeAsRoot "systemd-nspawn -D /mnt mkinitcpio -p linux-rt"
  fi

  executeAsRoot "umount /mnt/boot"
  executeAsRoot "umount /mnt"
}

update(){
    killall thttpd

    if ! /update/utilities/thttpd -p 8000 -d /update/EPC/ -l /update/EPC/server.log; then
        report_and_quit "E46 ePC update: Could not start http server on BBB..." "46";
    fi
    
    add_ping_test_to_nlhook

    for RETRYCOUNTER in {1..5}; do
        echo "restarting epc (try $RETRYCOUNTER)"
        executeAsRoot "systemctl --force --force reboot"

        if ! wait4playground; then
            report_and_quit "E45 ePC update: Reboot taking too long... timed out" "45"
        fi

        if cat /update/EPC/server.log | grep '"GET /update.tar HTTP/1.1" 200'; then
            rm /update/EPC/server.log
            killall thttpd
            return 0
        fi
    done

    killall thttpd
    rm /update/EPC/server.log
    report_and_quit "E47 ePC update: pulling update failed ..." "47"
    return 0
}

main () {
    check_preconditions
    update
    return 0
}

main
