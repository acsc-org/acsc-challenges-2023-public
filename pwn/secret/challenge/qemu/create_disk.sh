#!/bin/sh
set -e

dd if=/dev/zero of=./flag.img count=512
mkfs.ext2 ./flag.img
dd if=/dev/zero of=./flag-dist.img count=512
mkfs.ext2 ./flag-dist.img

mkdir -p ./flag/

mount flag.img ./flag/
echo "ACSC{4lw4ys_b3_c4r3fuL_1n_3rR0r_h4ndL1nG}" > ./flag/flag.txt
mv ./flag/flag.txt ./flag/flag-$(md5sum ./flag/flag.txt | awk '{print $1}').txt
umount ./flag/

mount flag-dist.img ./flag/
echo "ACSC{***** REDACTED *****}" > ./flag/flag-XXX.txt
umount ./flag/

rm -r ./flag/
