#!/bin/sh

PACKAGES=$(echo "@PI4_PACKAGES@" | tr ';' ' ')

apt-cache depends --recurse --no-recommends --no-suggests --no-conflicts --no-breaks --no-replaces --no-enhances $PACKAGES | \
  grep "^\w" | sort -u | grep -v "libacl1" | grep -v "chrony" | grep -v "libelogind0" | grep -v "^ntp$" | grep -v "ntpsec" | grep -v "openntpd"
