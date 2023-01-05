#!/bin/bash

ffmpeg -i aura.mp3 -codec:a libvorbis -qscale:a 8 aura.ogg
ffmpeg -i bliss.mp3 -codec:a libvorbis -qscale:a 8 bliss.ogg
ffmpeg -i heroes.mp3 -codec:a libvorbis -qscale:a 8 heroes.ogg
ffmpeg -i silence.mp3 -codec:a libvorbis -qscale:a 8 silence.ogg
ffmpeg -i soothing.mp3 -codec:a libvorbis -qscale:a 8 soothing.ogg
ffmpeg -i wonderful_time.mp3 -codec:a libvorbis -qscale:a 8 wonderful_time.ogg
