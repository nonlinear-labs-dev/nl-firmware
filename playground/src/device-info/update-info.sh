#!/bin/sh

cd ../..
git rev-parse --abbrev-ref HEAD > resources/version-infos/git-branch
git rev-list HEAD --count > resources/version-infos/git-commit-count
git rev-parse --short HEAD > resources/version-infos/git-head-ref
echo "[OKAY!] Wrote Version Info"