#!/bin/bash
FILES=~/SOUNDS/SDSUPERSYNTH/OLD/*
for f in $FILES
do
  filename_ext=$(basename -- "$f")
  extension="${filename_ext##*.}"
  filename="${filename_ext%.*}"
  echo $filename
  echo $extension
  echo "Processing $filename_ext file..."
  sox $filename_ext -c 1 -b 16 -r 44100 output.$filename_ext
done
