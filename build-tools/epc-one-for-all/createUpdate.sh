#!/bin/sh

set -x
set -e

OUTDIR="$1"
shift
DIR="$OUTDIR/update"

rm -rf $DIR
mkdir -p $DIR

for var in $@; do
  cp $var $DIR/
done

cat << EOF > $DIR/backdoor.sh
#!/bin/sh
set -e
set -x

mkdir -p /nloverlay

for dir in "os-overlay" "runtime-overlay" "work"; do
  rm -rf /nloverlay/\$dir
  mkdir -p /nloverlay/\$dir
done

mount -t overlay -o lowerdir=/lroot,upperdir=/nloverlay/os-overlay,workdir=/nloverlay/work overlay /mnt

mv /mnt/usr/lib/modules /tmp/
rm -rf /mnt/*
tar -C /mnt -xzf /nloverlay/update-scratch/update/c15-rootfs.tar.gz
mv /tmp/modules /mnt/usr/lib/
umount /mnt

for dir in "runtime-overlay" "work"; do
  rm -rf /nloverlay/\$dir
  mkdir -p /nloverlay/\$dir
done

/bin/sh /nloverlay/update-scratch/update/post-install.sh /nloverlay/os-overlay
EOF

BACKDOOR_CHECKSUM=$(sha256sum $DIR/backdoor.sh | cut -d " " -f 1)
touch $DIR/$BACKDOOR_CHECKSUM.sign
tar -C $OUTDIR -cf $OUTDIR/update.tar update
