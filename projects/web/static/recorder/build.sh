#!/bin/sh

set -e

cd /builddir

echo "1"

ls -la /builddir

tsc --project /builddir/tsconfig.json

echo "2"

rm -rf /builddir/bundle
mkdir -p /builddir/bundle
cp /src/*.html /builddir/bundle
cp /src/*.css /builddir/bundle
cp /src/*.ico /builddir/bundle
cp -r /src/fonts /builddir/bundle
cp -r /src/img /builddir/bundle
cp /builddir/recorder.js /builddir/bundle

cd /builddir/bundle && tar cfz /builddir/bundle.tar.gz ./*
rm -rf /builddir/bundle
