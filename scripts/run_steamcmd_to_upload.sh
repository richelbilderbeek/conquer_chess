#!/bin/bash
#
# Run steamcmd to create a new build
#
# Usage:
#
#   ./scripts/run_steamcmd_to_upload.sh
#
#

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/run_steamcmd_to_upload.sh"
    echo " "
    exit 42
fi

echo "Update build script"

sed -i "s/  \"Desc\".*/  \"Desc\" \"Upload at $(date)\"/g" steam/app_build_3873940.vdf

echo "Creating new build"

steamcmd +login richelbilderbeek +run_app_build /home/richel/GitHubs/conquer_chess/steam/app_build_3873940.vdf +quit

echo "Activate the new build on Steamworks."
read -p "Press enter when done"

echo "Start the game via Steam."
read -p "Press enter when done"

echo "Checking Steam install."

if ./scripts/check_steam_install.sh then
  echo "Everything seems to work :-)" 
fi



