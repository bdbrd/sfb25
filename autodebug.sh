#!/bin/bash
# how it works:
# Args are fed to QEMU, then GDB and everything else does shit automagically
# 1st arg: terminal name to spawn GDB
# 2nd arg: place to breakpoint in
termname=$1
breakpoint=$2
shift 2

qemu-system-x86_64 "$@" &

wait 1

"$termname" -e gdb -ex 'target remote localhost:1234' -ex 'break _start' -ex 'continue' build/SFB25.elf



