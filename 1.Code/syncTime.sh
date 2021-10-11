#!/bin/bash

{
    cat /dev/cu.usbserial-1D160
} &

sleep 3s
nowTime=$(date +%s)
nowTime=$(expr $nowTime + 28800)
nowTime=T$nowTime
echo $nowTime
echo -e $nowTime > /dev/cu.usbserial-1D160
