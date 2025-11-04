#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 \
  -audiodev coreaudio,id=snd0 \
  -machine pcspk-audiodev=snd0
