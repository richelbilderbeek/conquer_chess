#!/bin/bash
#
# Converts the PNG filesnames to lines for a Qt resource file,
# (game_resources.qrc, in the project's root folder)

ls *.jpg | awk '{ print "        <file>resources/textures/artwork/" $0 "</file>" }'