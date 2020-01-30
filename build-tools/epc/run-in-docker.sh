#!/bin/sh

set +x

BINARY_DIR=$1
shift
SOURCE_DIR=$1
shift

echo "#!/bin/sh" > /tmp/tmpscript.sh

for var in "$@"
do
    echo "$var" >> /tmp/tmpscript.sh
done

USER_ID=$(id -u $USER)
DOCKER_GROUP_ID=$(getent group docker | cut -d: -f3)
DOCKERNAME="nl-epc-build-environment-$USER"

modprobe fuse
docker run --rm -v /tmp:/script -v $BINARY_DIR:/workdir -v $SOURCE_DIR:/sources $DOCKERNAME bash /script/tmpscript.sh
