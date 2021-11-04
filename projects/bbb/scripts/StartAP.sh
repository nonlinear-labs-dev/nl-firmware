#!/bin/sh

echo "enabling and starting AP temporarily"
systemctl unmask accesspoint
systemctl start accesspoint