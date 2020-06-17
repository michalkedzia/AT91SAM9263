#!/bin/sh
~/sw/at91sam9263/arm-softmmu/qemu-system-arm  -M at91sam9263ek -pflash ~/sw/at91sam9263/virtual_lab/pflash.img -nographic -S -s -monitor /dev/tty 
reset

