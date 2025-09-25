#!/bin/bash
#
# Converts the PNG filesnames to lines for a Qt resource file,
# (game_resources.qrc, in the project's root folder)

ls *.png | awk '{ print "        <file>resources/textures/board_game/" $0 "</file>" }'