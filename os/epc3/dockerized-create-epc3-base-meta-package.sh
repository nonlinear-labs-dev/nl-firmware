#!/bin/sh

set -e

mkdir /epc3-meta

ln -s /src/install.sh /epc3-meta/install.sh

cat << EOF > /epc3-meta/PKGBUILD
pkgname=epc3-base-meta
pkgver="1.0.0"
pkgrel=1
pkgdesc="Nonlinear Labs Linux for 11i3... NUC systems"
arch=('any')
url="http://nonlinearlabs.de"
license=('MIT')
install=./install.sh
source=(
    'install-nlhook::file:///src/install/nlhook' 
    'install-oroot::file:///src/install/oroot' 
    'hook-nlhook::file:///src/hook/nlhook' 
    'hook-oroot::file:///src/hook/oroot')
sha256sums=(
    '$(sha256sum /src/install/nlhook | cut -d " " -f 1 )'
    '$(sha256sum /src/install/oroot | cut -d " " -f 1 )'
    '$(sha256sum /src/hook/nlhook | cut -d " " -f 1 )'
    '$(sha256sum /src/hook/oroot | cut -d " " -f 1 )')
depends=(
    $(cat /src/packages.txt | sed "s/\(.*\) \(.*\)/'\1=\2'/g")
    )

package() {
    mkdir -p \$pkgdir/usr/lib/initcpio/install
    mkdir -p \$pkgdir/usr/lib/initcpio/hooks
    cp \$srcdir/install-nlhook \$pkgdir/usr/lib/initcpio/install/nlhook
    cp \$srcdir/install-oroot \$pkgdir/usr/lib/initcpio/install/oroot
    cp \$srcdir/hook-nlhook \$pkgdir/usr/lib/initcpio/hooks/nlhook
    cp \$srcdir/hook-oroot \$pkgdir/usr/lib/initcpio/hooks/oroot
}
EOF

useradd bob
chmod 777 /epc3-meta
su - bob -c "cd /epc3-meta && makepkg -d -s -f"
mv /epc3-meta/epc3-base-meta-1.0.0-1-any.pkg.tar.zst /out
rm -rf /epc3-meta

