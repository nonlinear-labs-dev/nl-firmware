#!/bin/sh

set -e

mkdir /epc3-base

add_package() {
    for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
        for PLATFORM in "x86_64" "any"; do
            fileName="$1-$2-$PLATFORM.$ENDING"
            if [ -f /packages/$fileName ]; then
                echo "add /packages/$fileName to database"
                repo-add -q /epc3-base/epc3-base.db.tar.zst /packages/$fileName
                return 0
            fi
        done
    done

    return 1
}

while read -r package || [ -n "$package" ] ; do
  if [ ! -z "$package" ]; then
    name=$(echo $package | cut -f1 -d " ")
    version=$(echo $package | cut -f2 -d " ")
    add_package $name $version || return 1
  fi
done < /src/packages.txt


repo-add -q /epc3-base/epc3-base.db.tar.zst /out/epc3-base-meta-1.0.0-1-any.pkg.tar.zst
mv /epc3-base/* /out

rm -rf /epc3-base
