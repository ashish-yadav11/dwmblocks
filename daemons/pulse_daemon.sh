#!/bin/sh
# This script should be autostarted with X session
# It signals volume block to update on audio related events
pactl subscribe |
    while IFS='' read -r output ; do
        case $output in
            *" sink "*) sigdwmblocks 1 ;;
        esac
    done
