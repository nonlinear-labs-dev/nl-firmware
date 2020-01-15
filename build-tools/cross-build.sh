#!/bin/bash

function runShell() {
    PREPARE_COMMANDS="export HOME=/workdir/ccache"    
    docker run \
        -u $(id -u ${USER}):${DOCKER_GROUP_ID} \
        --rm \
        -v ${WORKDIR}:/workdir ${DOCKERNAME} \
            bash -c "$PREPARE_COMMANDS && $1"
    return $?
}

function quit() {
    echo $1

    if [ -n "${STARTSHELL}" ]; then 
        runShell "bash"
    fi

    exit 1
}

function printHelpAndQuit() {
    echo "$0 options..."
    echo " -w working folder => sources and object files will be put here"
    echo " -c local C15 folder => point to a local C15 folder in order to x-build it"
    echo " -b branchname => instead of pointing to local C15 folder, use a fresh checkout of the given branch"
    echo " -s => start a bash in the running docker after building, useful for debugging"
    echo " -t 'c|r' => buildroot-targets to build, c=C15, r=rootfs. Default is 'cr'"
    exit 0
}

TARGETS="pr"

while getopts ":w:c:b:st:h" opt; do
    case $opt in
        w) WORKDIR=$(readlink -f "$OPTARG")
        ;;
        c) C15DIR=$(readlink -f "$OPTARG")
        ;;
        b) BRANCH="$OPTARG"
        ;;
        s) STARTSHELL="1"
        ;;
        t) TARGETS="$OPTARG"
        ;;
        h) printHelpAndQuit
        ;;
        \?) quit "Invalid option -$OPTARG"
        ;;
    esac
done

function parseOpts() {
    DOCKERNAME=nl-cross-build-environment-${USER}
    DOCKER_GROUP_ID=`getent group docker | cut -d: -f3`
    USER_ID=`id -u $USER`
    SCRIPT_PATH=$(readlink -f "$0")
    SCRIPT_DIR=$(dirname "${SCRIPT_PATH}")

    echo "Working directory: $WORKDIR"
    echo "C15 directory: $C15DIR"
    echo "Branch: $BRANCH"
    echo "Script directory: $SCRIPT_DIR"
}

function checkPreconditions() {
    echo ${FUNCNAME[0]}
    if [ -n "$C15DIR" ] && [ -n "$BRANCH" ]; then
        quit "You cannot sepcify local c15 dir and branch the same time!"
    fi

    if [ -z "$C15DIR" ] && [ -z "$BRANCH" ]; then
        quit "You have to sepcify either local c15 dir (-c) or branch (-b)!"
    fi

    if [ -z "$WORKDIR" ]; then
        quit "You have to sepcify the working directory (-w)!"
    fi
}

function checkoutNonlinux() {
    echo ${FUNCNAME[0]}
    if [ ! -d "$WORKDIR/nonlinux" ]; then
        git -C "$WORKDIR" clone -b nonlinear_2020.01 --single-branch https://github.com/nonlinear-labs-dev/nonlinux.git || quit "Could not clone nonlinux repo!"
    fi
    git -C "$WORKDIR/nonlinux" fetch
    git -C "$WORKDIR/nonlinux" checkout nonlinear_2020.01 || quit "Could not checkout branch 'nonlinear_2016.05' in nonlinux repo!"
}

function prepareBuild() {
    echo ${FUNCNAME[0]}
    if [ -f "$WORKDIR/nonlinux/output/images/rootfs.tar.gz" ] && [ -d "$WORKDIR/nonlinux/output/build/C15-master" ]; then
        echo "Build looks already prepared, skipping..."
        return 0
    fi

    docker build \
        --build-arg user=${USER} \
        --build-arg user_id=${USER_ID} \
        --build-arg docker_group_id=${DOCKER_GROUP_ID} \
        -t ${DOCKERNAME} ${SCRIPT_DIR} || quit "Could not build ${DOCKERNAME}"

    COMMANDS="cd /workdir/nonlinux && \
              make nonlinear_defconfig && \
              make"

    runShell "$COMMANDS" || quit "Could not prepare build in docker." 
}

function injectC15Dir() {
    echo ${FUNCNAME[0]}
    shopt -s dotglob
    rsync -av --delete --exclude=buildroot-build $C15DIR/* $WORKDIR/nonlinux/output/build/C15-master/
}

function injectC15Branch() {
    echo ${FUNCNAME[0]}
    if [ ! -d "$WORKDIR/C15" ]; then
        git -C "$WORKDIR" clone -b $BRANCH --single-branch https://github.com/nonlinear-labs-dev/C15.git || quit "Could not clone C15 repo!"
    fi 

    C15DIR="$WORKDIR/C15" 
    
    git -C "$C15DIR" fetch
    git -C "$C15DIR" checkout $BRANCH || quit "Could not checkout branch '$BRANCH' in C15 repo!"
    git -C "$C15DIR" pull origin $BRANCH || quit "Could not pull branch '$BRANCH' in C15 repo!"

    injectC15Dir
}

function doFinalBuild() {
    echo ${FUNCNAME[0]}

    COMMANDS="cd /workdir/nonlinux"

    if [[ ${TARGETS} =~ "c" ]]
    then
        COMMANDS="${COMMANDS} && make C15-clean-for-reconfigure && make C15-rebuild"
    fi

    if [[ ${TARGETS} =~ "r" ]]
    then
        COMMANDS="${COMMANDS} && make"  
    fi
    
    runShell "$COMMANDS" || quit "Could not do final build step in docker." 
}

main() {
    echo ${FUNCNAME[0]}
    parseOpts
    checkPreconditions
    checkoutNonlinux
    prepareBuild
    
    if [ -n "$C15DIR" ]; then
        injectC15Dir
    fi

    if [ -n "$BRANCH" ]; then
        injectC15Branch
    fi

    doFinalBuild
      
    echo "Everything worked perfectly. Your stuff is in ${WORKDIR}/nonlinux/output/target."
    echo "maybe: rsync -zz -av /home/hhoegelo/dev/builds/c15-cross/nonlinux/output/target/usr/ root@192.168.8.2:/usr??"

    if [ -n "${STARTSHELL}" ]; then 
        runShell "bash  "
    fi 
    exit 0
}

main
