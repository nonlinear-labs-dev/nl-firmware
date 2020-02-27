#!/bin/sh
#
# Author:       Anton Scmied
# Date:         12.02.2020
# TODO:         - deploy lpc update

# vom Cmake übergebene Pfade zu den .tarS
EPC_UPDATE=$1
BBB_UPDATE=$2
#LPC_CORE_0=$3
#LPC_CORE_1=$4

CURRENT_DIR=$("pwd")

fail_and_exit() {
    clean_up
    echo "Failed to created an update!"
    exit 1
}

check_preconditions () {
    if [ -z "$EPC_UPDATE" ]; then echo "ePC update missing..." && return 1; fi
    if [ -z "$BBB_UPDATE" ]; then echo "BBB update missing..." && return 1; fi
#    if [ -z "$LPC_CORE_0" ]; then echo "LPC update missing..." && return 1; fi
#    if [ -z "$LPC_CORE_1" ]; then echo "LPC update missing..." && return 1; fi
    return 0
}

create_update_file_structure() {
    echo "Creating Update Structure..."
    if mkdir $CURRENT_DIR/nonlinear-c15-update \
        && mkdir $CURRENT_DIR/nonlinear-c15-update/BBB \
        && mkdir $CURRENT_DIR/nonlinear-c15-update/EPC \
        && mkdir $CURRENT_DIR/nonlinear-c15-update/LPC \
        && mkdir $CURRENT_DIR/nonlinear-c15-update/utilities; then
        echo "Creating Update Structure done."
        return 0
    fi
    echo "Creating Update Structure failed."
    return 1
}

deploy_updates() {
    echo "Deploying updates..."
    if cp $EPC_UPDATE $CURRENT_DIR/nonlinear-c15-update/EPC/update.tar \
        && chmod 666 $CURRENT_DIR/nonlinear-c15-update/EPC/update.tar \
#        && cp $LPC_CORE_0 $CURRENT_DIR/nonlinear-c15-update/LPC/ \
#        && chmod 666 $CURRENT_DIR/nonlinear-c15-update/LPC/M0_project.bin\
#        && cp $LPC_CORE_1 $CURRENT_DIR/nonlinear-c15-update/LPC/ \
#        && chmod 666 $CURRENT_DIR/nonlinear-c15-update/LPC/M4_project.bin\
        && cp $BBB_UPDATE $CURRENT_DIR/nonlinear-c15-update/BBB/ \
        && chmod 666 $CURRENT_DIR/nonlinear-c15-update/BBB/rootfs.tar.gz; then
        echo "Deploying updates done."
        return 0
     fi
     echo echo "Deploying updates failed."
     return 1
}

deploy_scripts() {
    echo "Deploying update scripts..."
    if cp $CURRENT_DIR/update_scripts/run.sh $CURRENT_DIR/nonlinear-c15-update/ \
        && chmod 777 $CURRENT_DIR/nonlinear-c15-update/run.sh \
        && cp $CURRENT_DIR/update_scripts/bbb_update.sh $CURRENT_DIR/nonlinear-c15-update/BBB/ \
        && chmod 777 $CURRENT_DIR/nonlinear-c15-update/BBB/bbb_update.sh \
        && cp $CURRENT_DIR/update_scripts/epc_update.sh $CURRENT_DIR/nonlinear-c15-update/EPC/ \
        && chmod 777 $CURRENT_DIR/nonlinear-c15-update/EPC/epc_update.sh \
        && cp $CURRENT_DIR/update_scripts/lpc_update.sh $CURRENT_DIR/nonlinear-c15-update/LPC/ \
        && chmod 777 $CURRENT_DIR/nonlinear-c15-update/LPC/lpc_update.sh; then
        echo "Deploying update scripts done."

        return 0
     fi
     echo echo "Deploying update scripts failed."
     return 1
}

get_tools_from_rootfs() {
    echo "Getting tools from rootfs..."
    mkdir -p $CURRENT_DIR/rootfs && tar -xf $BBB_UPDATE -C $CURRENT_DIR/rootfs

    tools=(sshpass text2soled netcat rsync socat tinyhttpd lighttpd thttpd)
    for i in "${tools[@]}"; do
        cp $(find $CURRENT_DIR/rootfs/usr -type f -name "$i") $CURRENT_DIR/nonlinear-c15-update/utilities/
    done

    if [ $? -eq 0 ]; then
        echo "Getting tools from rootfs done."
        return 0
    fi
    echo "Getting tools from rootfs failed."
    return 1
}


create_update_tar () {
    echo "Creating nonlinear-c15-update.tar..."
    if cd $CURRENT_DIR/nonlinear-c15-update/ && tar -cf nonlinear-c15-update.tar * \
        && mv $CURRENT_DIR/nonlinear-c15-update/nonlinear-c15-update.tar $CURRENT_DIR; then
        echo "Creating nonlinear-c15-update.tar done."
        return 0
    fi
    echo "Creating nonlinear-c15-update.tar failed."
    return 1
}


calc_checksum() {
    echo "Calc checksum..."
    if (cd $CURRENT_DIR/nonlinear-c15-update/ && touch $(sha256sum ./nonlinear-c15-update.tar | grep -o "^[^ ]*").sign); then
        echo "Calc checksum done."
        return 0
    fi
    echo "Calc checksum failed."
    return 1
}

clean_up() {
    echo "Cleaning up..."
    if rm -rf $CURRENT_DIR/nonlinear-c15-update/ && rm -rf $CURRENT_DIR/rootfs/; then
        echo "Cleaning up done."
        return 0
    fi
    echo "Cleaning up failed."
    return 1
}


main() {
    check_preconditions || fail_and_exit
    create_update_file_structure || fail_and_exit
    deploy_updates || fail_and_exit
    deploy_scripts || fail_and_exit
    get_tools_from_rootfs || fail_and_exit
    create_update_tar || fail_and_exit
    clean_up || fail_and_exit
}

main
