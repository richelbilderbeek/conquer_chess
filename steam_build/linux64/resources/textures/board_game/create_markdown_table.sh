#!/bin/bash
#
# Converts the PNG filesnames to a Markdown table

ls *.png | awk '{ print "| <" $0 "> | ![" $0 "](" $0 ") | [kenney.nl](https://www.kenney.nl/assets/board-game-icons) | " }'