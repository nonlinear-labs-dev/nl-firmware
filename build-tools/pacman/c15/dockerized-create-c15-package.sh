#!/bin/sh

set -e

useradd -u @USER_ID@ bob
echo 'bob ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

for dir in /c15 /out/build /home/bob /out/ccache; do
  mkdir -p $dir
  chown bob:bob $dir
done

ln -s /out/ccache /home/bob/.ccache

NUM_CORES=$(nproc --all)

cat << EOF > /c15/install.sh
post_install() {
  echo "en_US@nonlinear.UTF-8 UTF-8" > /etc/locale.gen
  echo "LANG=en_US@nonlinear.UTF-8" > /etc/locale.conf
  locale-gen
  systemctl enable playground
  systemctl enable audio-engine
  systemctl start playground
  systemctl start audio-engine
}
EOF

chmod +x /c15/install.sh

cat << EOF > /c15/PKGBUILD
pkgname=c15
pkgver="1.0.0"
pkgrel=1
pkgdesc="Nonlinear Labs Applications"
arch=('any')
url="http://nonlinearlabs.de"
license=('MIT')

depends=(
'alsa-lib'
'alsa-tools'
'alsa-utils'
'boost'
'cpupower'
'flac'
'freetype2'
'glibmm>=2.4'
'grep'
'gzip'
'libpng'
'libsoup>=2.4'
'lsof'
'networkmanager'
'util-linux-libs'
'shared-mime-info'
'sed'
'sudo'
'sshfs'
'systemd'
'systemd-sysvcompat'
)

build() {
  DESTDIR="\$pkgdir" cmake -B /out/build -S /src -DTARGET_PLATFORM=epc2 -DCMAKE_BUILD_TYPE=Release -DBUILD_EPC_SCRIPTS=On -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_WEB=Off
  DESTDIR="\$pkgdir"  cmake --build /out/build --parallel $NUM_CORES
}

package() {
  mkdir -p \$pkgdir/usr/local/C15/web
  sudo DESTDIR="\$pkgdir" cmake --install /out/build
  sudo tar -xzf /web/web.tar.gz -C \$pkgdir/usr/local/C15/web
}

install=install.sh
EOF

su - bob -c "git config --global --add safe.directory /src"
su - bob -c "cd /c15 && makepkg -d -s -f"
mv /c15/c15-1.0.0-1-any.pkg.tar.zst /out

