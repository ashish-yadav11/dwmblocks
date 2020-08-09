#!/bin/sh
pactl subscribe |
    while read -r output ; do
        [ -z "${output##*sink *}" ] && sigdsblocks 1
    done
