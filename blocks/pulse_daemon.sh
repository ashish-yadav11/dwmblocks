#!/bin/dash
pactl subscribe |
    while read -r output ; do
        if [ -z "${output##*sink *}" ] ; then
            sigdwmblocks 1
        fi
    done
