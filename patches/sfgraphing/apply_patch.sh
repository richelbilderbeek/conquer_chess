#!/bin/sh
# Apply the SFML 3 port to SFGraphing, unless it is already applied.
#
# Usage: apply_patch.sh <SFGraphing folder>
set -e
patch_file="$(cd "$(dirname "$0")" && pwd)/0001-Port-to-SFML-3.patch"
cd "$1"
if git apply --reverse --check "$patch_file" 2>/dev/null; then
  echo "Already applied: $patch_file"
else
  git apply "$patch_file"
  echo "Applied: $patch_file"
fi
