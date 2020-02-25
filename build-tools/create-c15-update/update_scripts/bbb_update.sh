#!/bin/sh

#
# Author: Anton Schmied
# version : 2.0
#
# ----------- install a BBB Updates from USB-Stick ---------
#
# This script will transfer the /preset-manager/* and settings.xml to the ePC
# and update the BBB excluding the /update directory
#

EPC_IP=$1
BBB_IP=$2

report_and_quit(){
    printf "$1" >> /update/errors.log
    exit $2
}

executeAsRoot() {
    echo "sscl" | /update/utilities/sshpass -p 'sscl' ssh -o ConnectionAttempts=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no sscl@$EPC_IP \
        "sudo -S /bin/bash -c '$1' 1>&2 > /dev/null"
    return $?
}

check_preconditions(){
    [ -z "$EPC_IP" ] && report_and_quit "E81: Usage: $EPC_IP <IP-of-ePC> wrong ..." "81"
    ping -c1 $EPC_IP 1>&2 > /dev/null || report_and_quit "E82: Can't ping ePC on $EPC_IP ..." "82"
    executeAsRoot "exit" || report_and_quit "E83: Can't logon to ePC OS" "83"
    executeAsRoot "mountpoint -q /persistent" || report_and_quit "E54 BBB update: user partition not mounted" "54"

    #TODO: check if we need to move any files and set boolean for move files
    executeAsRoot "ls -A /persistent/preset-manager/"
    executeAsRoot "[ -e /settings.xml ]"
}

move_files(){
    if [ -d /internalstorage/preset-manager ] && [ "$(ls -A /internalstorage/preset-manager/)" ]; then
        executeAsRoot "scp -r root@$BBB_IP:/internalstorage/preset-manager/ /persistent/preset-manager"
        rm -rf /internalstorage/preset-manager/*
        rm -rf /internalstorage/preset-manager
    fi

    if [ -e /settings.xml ]; then
        executeAsRoot "scp root@$BBB_IP:/settings.xml /persistent/settings.xml"
        rm /settings.xml
    fi
}

update(){
#   unpack rootfs
    mkdir /update/BBB/rootfs
    tar -C /update/BBB/rootfs -xvf rootfs.tar
    update/utilities/rsync -cv --exclude '/update/' /update/BBB/rootfs/ / # not quite!
}





main() {
    check_preconditions
    move_files
    update

}


main



