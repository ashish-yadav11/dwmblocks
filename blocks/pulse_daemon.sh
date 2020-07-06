#!/bin/dash
pactl subscribe |
    while read -r output ; do
        if [ -z "${output##*sink *}" ] ; then
            sigdsblocks 1
        fi
    done
