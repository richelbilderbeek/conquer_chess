#!/bin/bash
#
# Downloads chess games 
#
#
wget https://database.lichess.org/standard/lichess_db_standard_rated_2013-01.pgn.bz2

# From https://fedingo.com/how-to-extract-bz2-file-in-linux/
bzip2 -d lichess_db_standard_rated_2013-01.pgn.bz2
