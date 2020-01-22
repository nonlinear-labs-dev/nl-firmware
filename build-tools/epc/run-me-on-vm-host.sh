#!/bin/sh

check_preconditions() {
    echo "Checking preconditions..."
    AP="$1"
    
    if [ ! uuid ]; then 
        echo "UUID binary not installed"
        return 1
    fi

    if [ ! -f ${AP} ]; then
        echo "${AP} not found."
        echo "usage: ./run-me-on-vm-host.sh path-to-folder-containing-AP-Linux-V.4.0.iso"
        echo "Checking preconditions failed."
        return 1
    fi 
        
    echo "Checking preconditions done."
    return 0
}

create_vm() {
    PWD=`pwd`
    TIMESTAMP=`date +%m-%d-%Y-%H:%M:%S`
    VM_NAME="ePC-${TIMESTAMP}"
    MACHINE_FOLDER="${PWD}/machines/${VM_NAME}"
    mkdir -p ${MACHINE_FOLDER}
    MACHINE_PATH="${MACHINE_FOLDER}/${VM_NAME}.vbox"
    MACHINE_ID=`uuid`
    DVD_ID=`uuid`

    EXISTING_DVD_ID=`vboxmanage list dvds | grep -F "$AP" -B 4 | grep "^UUID" | grep -o " [^ ]*" | grep -o "[^ ]*"`
    echo "EXISTING_DVD_ID = ${EXISTING_DVD_ID}"

    if [ -n "${EXISTING_DVD_ID}" ]; then 
        DVD_ID=${EXISTING_DVD_ID}
        echo "DVD already registered as ${EXISTING_DVD_ID}"
    fi

    vboxmanage createmedium disk --filename ${MACHINE_FOLDER}/ePC-disk001.vmdk --size 65536 --format VMDK 
    HARDDISK_ID=`vboxmanage list hdds | grep -F "${MACHINE_FOLDER}/ePC-disk001.vmdk" -B 4 | grep "^UUID" | grep -o " [^ ]*" | grep -o "[^ ]*"`

    echo "HARDDISK_ID = ${HARDDISK_ID}"
    echo "DVD_ID = ${DVD_ID}"

    cp ./ePC.vbox.in ${MACHINE_PATH}
    sed -E "s@MACHINE_NAME@${VM_NAME}@g" -i ${MACHINE_PATH}
    sed -E "s@VM_NAME@${VM_NAME}@g" -i ${MACHINE_PATH}
    sed -E "s@MACHINE_ID@${MACHINE_ID}@g" -i ${MACHINE_PATH}
    sed -E "s@HARDDISK_ID@${HARDDISK_ID}@g" -i ${MACHINE_PATH}
    sed -E "s@DVD_ID@${DVD_ID}@g" -i ${MACHINE_PATH}
    sed -E "s@DVD_PATH@${AP}@g" -i ${MACHINE_PATH}

    vboxmanage registervm ${MACHINE_PATH}

    return $?
}

start_vm() {
    echo "Starting VM..."
    vboxmanage startvm ${VM_NAME}
}

choose_boot_option() {
    sleep 10
    vboxmanage controlvm ${VM_NAME} keyboardputscancode 1c 9c
}

start_script() {
    echo ""
   # sleep 120
   # vboxmanage controlvm ${VM_NAME} keyboardputstring 'curl -L "https://github.com/nonlinear-labs-dev/Audiophile2NonLinux/raw/master/runme.sh" | sh'
   # vboxmanage controlvm ${VM_NAME} keyboardputscancode 1c 9c
}

main() {
    if check_preconditions $1; then
        create_vm && start_vm && choose_boot_option && start_script
    fi
}


main $1
