#!/bin/sh
pactl subscribe |
    while read -r output ; do
        case $output in
            *"sink "*) sigdwmblocks 1 ;;
        esac
    done
