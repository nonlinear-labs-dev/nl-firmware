#!/bin/sh

set -e
set -x

BINARY_DIR=$(realpath $1)
shift
SOURCE_DIR=$(realpath $1)
shift

SSH_DIR=$(realpath "$HOME/.ssh")
USER_ID=$(id -u)

SCRIPT="
set -e
set -x
mkdir -p /docker-ssh
rsync -a /host-ssh/ /docker-ssh
chown -R root /docker-ssh
if ! grep \"/docker-ssh\" /etc/ssh/ssh_config; then
 echo \"IdentityFile /docker-ssh/id_rsa\" >> /etc/ssh/ssh_config
fi

useradd -m -u ${USER_ID} bob-the-builder
cd /home/bob-the-builder

runuser -l bob-the-builder -c \"
"

for var in "$@"
do
    SCRIPT="$SCRIPT
    $var"
done

SCRIPT="$SCRIPT\""

tty && TTY=" -ti "

DOCKERNAME="nl-cross-build-environment"
docker run $TTY --privileged --rm -v $SSH_DIR:/host-ssh -v $BINARY_DIR:/workdir -v $SOURCE_DIR:/sources $DOCKERNAME bash -c "$SCRIPT"
