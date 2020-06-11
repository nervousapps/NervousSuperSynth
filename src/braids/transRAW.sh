#!/bin/bash
FILES=~/SOUNDS/SDSUPERSYNTH/NEWFILES/*
for f in $FILES
do
  filename_ext=$(basename -- "$f")
  extension="${filename_ext##*.}"
  filename="${filename_ext%.*}"
  echo $filename
  echo $extension
  echo "Processing $filename_ext file..."
  sox $filename_ext  ../OUTPUT/$filename.RAW
done
