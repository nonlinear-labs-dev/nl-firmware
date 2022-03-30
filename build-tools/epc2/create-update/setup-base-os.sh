#!/bin/sh

set -e

PACKAGES_COLLECTION=""

declare -A versions

collect_package() {
    name="$1"
    version="$2"
    
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

collect_packages() {
  for name in "${!versions[@]}"; do
    version="${versions[$name]}"
      
    EXISTING=$(pacman -Qi $name 2> /dev/null | grep Version | sed 's/Version\s*: //g')
    if [ ! "$EXISTING" = "$version" ]; then
      echo "Installing $name in version $version."
      collect_package $name $version || return 1
    fi
  done
}

get_package_with_highest_version() {
  package="$1"
  if [ ! -z "$package" ]; then
    name=$(echo $package | cut -f1 -d " ")
    version=$(echo $package | cut -f2 -d " ")
  
    if [ -z ${versions[$name]} ]; then
      versions[$name]=$version
    elif [[ ${versions[$name]} < $version ]]; then
      versions[$name]=$version
    fi
  fi
}

install_packages() {
    echo "Installing packages"
    
    while read -r package; do get_package_with_highest_version "$package"; done < /base-packages.txt
    while read -r package; do get_package_with_highest_version "$package"; done < /build-packages.txt
    
    collect_packages
    
    PACKAGES_COLLECTION=$(echo $PACKAGES_COLLECTION | tr ' ' '\n' | sort -u | tr '\n' ' ')
    pacman --noconfirm -Sy
    echo "Packages $PACKAGES_COLLECTION" 
    yes | pacman -U $PACKAGES_COLLECTION
    return 0
}

install_packages