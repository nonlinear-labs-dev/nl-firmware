#!/bin/sh

set -e

BASE_URLS="$1"
TARGET_DIR="$2"
PACKAGES="$3"

download_package() {
    name="$1"
    version="$2"

    for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
        for PLATFORM in "x86_64" "any"; do
            fileName="$name-$version-$PLATFORM.$ENDING"
            if [ -f $TARGET_DIR/$fileName ]; then
                return 0
            fi
        done
    done

    for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
        for PLATFORM in "x86_64" "any"; do
            fileName="$name-$version-$PLATFORM.$ENDING"
            for BASE_URL in $BASE_URLS; do
                if wget -q $BASE_URL/$fileName -O $TARGET_DIR/$fileName.tmp > /dev/null; then
                    mv $TARGET_DIR/$fileName.tmp $TARGET_DIR/$fileName
                    return 0
                else
                    rm $TARGET_DIR/$fileName.tmp
                fi
            done
        done
    done

    echo "Could not download package $name in version $version."
    return 1
}

download_packages() {
    while read -r package || [ -n "$package" ] ; do
      if [ ! -z "$package" ]; then
        name=$(echo $package | cut -f1 -d " ")
        version=$(echo $package | cut -f2 -d " ")
        download_package $name $version || return 1
      fi
    done < $PACKAGES

    return 0
}

download_packages || exit 1
