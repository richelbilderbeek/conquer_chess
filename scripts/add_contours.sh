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

convert resources/bw.png -bordercolor none -border 10x10 -background black -alpha background -channel A -blur 0x10 -level 0,70% resources/bw_with_contour.png
convert resources/kw.png -bordercolor none -border 10x10 -background black -alpha background -channel A -blur 0x10 -level 0,70% resources/kw_with_contour.png
convert resources/nw.png -bordercolor none -border 10x10 -background black -alpha background -channel A -blur 0x10 -level 0,70% resources/nw_with_contour.png
convert resources/pw.png -bordercolor none -border 10x10 -background black -alpha background -channel A -blur 0x10 -level 0,70% resources/pw_with_contour.png
convert resources/qw.png -bordercolor none -border 10x10 -background black -alpha background -channel A -blur 0x10 -level 0,70% resources/qw_with_contour.png
convert resources/rw.png -bordercolor none -border 10x10 -background black -alpha background -channel A -blur 0x10 -level 0,70% resources/rw_with_contour.png
