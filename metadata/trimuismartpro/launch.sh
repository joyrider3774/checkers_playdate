#!/bin/sh
echo $0 $*
PROGDIR=/mnt/SDCARD/Apps/checkers

cd $PROGDIR
HOME=$PROGDIR/ ./game -f
