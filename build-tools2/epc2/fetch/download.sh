#!/bin/sh

set -e

BASE_URL="$1"
PACKAGE_NAME="$2"
PACKAGE_VERSION="$3"
TARGET_DIR="$4"
SHA="$5"

if [ -e $TARGET_DIR/$SHA ]; then
  exit 0
fi

mkdir -p $TARGET_DIR/stamps

for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
  for PLATFORM in "x86_64" "any"; do
    if wget -q  $BASE_URL/$PACKAGE_NAME-$PACKAGE_VERSION-$PLATFORM.$ENDING -O $TARGET_DIR/$PACKAGE_NAME-$PACKAGE_VERSION-$PLATFORM.$ENDING > /dev/null; then
      touch $TARGET_DIR/stamps/$SHA
      exit 0
    else
      rm $TARGET_DIR/$PACKAGE_NAME-$PACKAGE_VERSION-$PLATFORM.$ENDING
    fi
  done
done

echo "Could not fetch package $PACKAGE_NAME in version $PACKAGE_VERSION."
exit 1