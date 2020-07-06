#!/bin/dash
case "$1" in
    1) setsid -f termite -e "htop -s PERCENT_CPU" ;;
    2) setsid -f termite -e "htop" ;;
    3) setsid -f termite -e "htop -s PERCENT_MEM" ;;
esac
