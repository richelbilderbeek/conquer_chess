#!/bin/bash
convert black_square.png -alpha set -channel A -evaluate set 50% black_square_semitransparent.png
convert white_square.png -alpha set -channel A -evaluate set 50% white_square_semitransparent.png
convert black_black.png -alpha set -channel A -evaluate set 50% black_black_semitransparent.png
convert white_black.png -alpha set -channel A -evaluate set 50% white_black_semitransparent.png
convert black_white.png -alpha set -channel A -evaluate set 50% black_white_semitransparent.png
convert white_white.png -alpha set -channel A -evaluate set 50% white_white_semitransparent.png
