#!/bin/sh
ICON="\x0c\x0b"
read -r capacity </sys/class/power_supply/BAT0/capacity
printf "$ICON%s%%" "$capacity"
