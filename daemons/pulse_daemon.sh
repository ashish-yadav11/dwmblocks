#!/bin/sh
# autostart this on startup for volume block to work
pactl subscribe |
    while read -r output ; do
        case $output in
            *"sink "*) sigdwmblocks 1 ;;
        esac
    done
