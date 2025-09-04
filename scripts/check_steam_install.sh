#!/bin/bash
#
# Check the Steam install
#
# Usage:
#

cc_install_folder=~/.local/share/Steam/steamapps/common/conquer_chess

echo "Conquer Chess Steam install folder: ${cc_install_folder}"
echo "Content of Conquer Chess install folder:"
echo " "
ls ${cc_install_folder}
echo " "

if [ -d ${cc_install_folder}/EmptySteamDepot ]; then
  echo "ERROR: Conquer Chess Steam install has no content."
  exit 42
fi

if [ ! -f "${cc_install_folder}/conquer_chess" ]; then
  echo "ERROR: Conquer Chess executable is not found."
  exit 43
fi

if [ ! -d "${cc_install_folder}/resources" ]; then
  echo "ERROR: Conquer Chess resources are not found."
  exit 43
fi

echo "All seems OK :-)"
