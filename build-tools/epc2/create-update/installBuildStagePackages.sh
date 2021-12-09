#!/bin/bash

PACKAGES_COLLECTION=""

collect_package() {
    name="$1"
    
    for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
        for PLATFORM in "x86_64" "any"; do
            fileName="$name-$version-$PLATFORM.$ENDING"
            if [ -f /packages/$fileName ]; then
              PACKAGES_COLLECTION="$PACKAGES_COLLECTION /packages/$fileName"
              return 0
            fi
        done
    done
        
    echo "Could not install package $name in version $version."
    return 1
}

install_package() {
  package="$1"
  if [ ! -z "$package" ]; then
    name=$(echo $package | cut -f1 -d " ")
    version=$(echo $package | cut -f2 -d " ")
    
    EXISTING=$(pacman -Qi $name 2> /dev/null | grep Version | sed 's/Version\s*: //g')
    if [ ! "$EXISTING" = "$version" ]; then
      echo "Installing $name in version $version."
      collect_package $name $version || return 1
    fi
  fi
}

install_packages() {
    while read -r package; do install_package "$package"; done < /base-packages.txt
    while read -r package; do install_package "$package"; done < /build-packages.txt
        
    PACKAGES_COLLECTION=$(echo $PACKAGES_COLLECTION | tr ' ' '\n' | sort -u | tr '\n' ' ')
    pacman --noconfirm -U $PACKAGES_COLLECTION
    return 0
}

install_packages
exit $?