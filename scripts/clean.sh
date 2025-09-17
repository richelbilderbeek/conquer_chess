#!/bin/bash

if [[ "$PWD" =~ scripts$ ]]; then
    echo "FATAL ERROR."
    echo "Please run the script from the project root. "
    echo "Present working director: $PWD"
    echo " "
    echo "Tip: like this"
    echo " "
    echo "  ./scripts/clean.sh"
    echo " "
    exit 42
fi

make clean
rm -f Makefile
rm -f Makefile.Debug
rm -f Makefile.Release
rm -f game
rm -f debug
rm -f release
rm -f ./*.gcda
rm -f ./*.gcno
rm conquer_chess
rm tmp_*
rm tmp.*
rm *.wkt
rm -f conquer_chess_error.txt
