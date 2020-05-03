#!/bin/bash

NAME=GARNIER_PIERRE
EXT=zip
DATE=`date +%d-%m-%y`
FILENAME=${NAME}_${DATE}.${EXT}


zip -r ${FILENAME} final
cp ${FILENAME} /mnt/c/WSL