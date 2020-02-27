#!/bin/sh

#
# Author: Anton Schmied
# version : 2.0
#
# ----------- install a ePC Updates from USB-Stick ---------
#
# This scripts will serve the epc-update.tar via thttpd on the BBB and
# restart the ePC, which will evoke the Update

EPC_IP=$1
TIMEOUT=60

report_and_quit(){
    printf "$1" >> /update/errors.log
    exit $2
}

executeAsRoot() {
    echo "sscl" | /update/utilities/sshpass -p 'sscl' ssh -o ConnectionAttempts=1 -o ConnectTimeout=1 -o StrictHostKeyChecking=no sscl@$EPC_IP \
        "sudo -S /bin/bash -c '$1' " # 1>&2 > /dev/null"
    return $?
}

wait4response() {
    COUNTER=1
    while [[ ! $COUNTER -gt $TIMEOUT ]]; do
        echo "awaiting reboot ... $COUNTER/$TIMEOUT"
        sleep 1

        rm /root/.ssh/known_hosts 1>&2 > /dev/null; executeAsRoot "exit"
        [ $? -eq 0 ] && break

        ((COUNTER = COUNTER + 1))
    done
    if [ $(( COUNTER - 1)) -eq $TIMEOUT ]; then return 1; fi
    return 0
}


check_preconditions(){
    [ -z "$EPC_IP" ] && report_and_quit "E81: Usage: $EPC_IP <IP-of-ePC> wrong ..." "81"
    ping -c1 $EPC_IP 1>&2 > /dev/null || report_and_quit "E82: Can't ping ePC on $EPC_IP ..." "82"
    executeAsRoot "exit" || report_and_quit "E83: Can't logon to ePC OS ..." "83"
}

update(){
    kill $(pidof 'thttpd')       # safe???
    /update/utilities/thttpd -p 8000 -d /update/EPC/ -l /update/EPC/server.log
    if [ $? -ne 0 ]; then report_and_quit "E46 ePC update: Could not start server on BBB..." "46"; fi
    SERVER_PID=$(pidof 'thttpd')

    rm /root/.ssh/known_hosts && executeAsRoot "sudo reboot"
    wait4response
    if [ $? -ne 0 ]; then kill $SERVER_PID; report_and_quit "E45 ePC update: Reboot taking too long... timed out" "45"; fi

    kill $SERVER_PID
    cat /update/EPC/server.log | grep '"GET /update.tar HTTP/1.1" 200'
    if [ $? -ne 0 ]; then rm /update/EPC/server.log; report_and_quit "E47 ePC update: deploying update failed ..." "47"; fi

    rm /update/EPC/server.log
    return 0
}

main () {
    check_preconditions
    update

    return 0
}

main
