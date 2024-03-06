#!/bin/sh
timeout --foreground 300 qemu-system-x86_64 \
    -m 64M \
    -nographic \
    -kernel bzImage \
    -initrd rootfs.cpio \
    -drive file=./flag.img,if=virtio,format=raw,readonly=on \
    -append "console=ttyS0 loglevel=3 oops=panic panic=-1 pti=on kaslr" \
    -no-reboot \
    -cpu kvm64,smap,smep \
    -monitor /dev/null \
    -net nic,model=virtio \
    -net user