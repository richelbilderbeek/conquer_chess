#!/bin/bash
# 
# sudo apt install ffmpeg
# 

ffmpeg -i themba_1.mp4 themba_1.mp3
ffmpeg -i themba_1.mp4 -vn -acodec libvorbis themba_1.ogg

ffmpeg -i themba_2.mp4 themba_2.mp3
ffmpeg -i themba_2.mp4 -vn -acodec libvorbis themba_2.ogg
