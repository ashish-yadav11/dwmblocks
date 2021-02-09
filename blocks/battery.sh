#!/bin/sh
ICON=""
read -r capacity </sys/class/power_supply/BAT0/capacity
printf "$ICON%s%%" "$capacity"
