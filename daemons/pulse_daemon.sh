#!/bin/sh
pactl subscribe |
    while read -r output ; do
        [ -z "${output##*sink *}" ] && sigdwmblocks 1
    done
