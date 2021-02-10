#!/bin/sh
# This script should be autostarted on startup
# It signals volume block to update on audio related events
pactl subscribe |
    while read -r output ; do
        case $output in
            *"sink "*) sigdwmblocks 1 ;;
        esac
    done
