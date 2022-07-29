#!/bin/bash
# Add a contour to a picture
#
# Usage:
#
#   ./scripts/add_contours.sh
#
convert resources/bb.png -bordercolor none -border 10x10 -background white -alpha background -channel A -blur 0x10 -level 0,70% resources/bb_with_contour.png
convert resources/kb.png -bordercolor none -border 10x10 -background white -alpha background -channel A -blur 0x10 -level 0,70% resources/kb_with_contour.png
convert resources/nb.png -bordercolor none -border 10x10 -background white -alpha background -channel A -blur 0x10 -level 0,70% resources/nb_with_contour.png
convert resources/pb.png -bordercolor none -border 10x10 -background white -alpha background -channel A -blur 0x10 -level 0,70% resources/pb_with_contour.png
convert resources/qb.png -bordercolor none -border 10x10 -background white -alpha background -channel A -blur 0x10 -level 0,70% resources/qb_with_contour.png
convert resources/rb.png -bordercolor none -border 10x10 -background white -alpha background -channel A -blur 0x10 -level 0,70% resources/rb_with_contour.png
