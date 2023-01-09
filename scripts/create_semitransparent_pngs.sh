#!/bin/bash
convert d.png -alpha set -channel A -evaluate set 50% dark_square_semitransparent.png
convert l.png -alpha set -channel A -evaluate set 50% light_square_semitransparent.png
