#!/bin/sh
case $1 in
    1) exec termite -e "htop -s PERCENT_CPU" ;;
    2) exec termite -e "htop" ;;
    3) exec termite -e "htop -s PERCENT_MEM" ;;
esac
