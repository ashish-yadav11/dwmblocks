#!/bin/sh

ICONn="\x0c\x0b" # icon for normal temperatures
ICONc="\x0d\x0b" # icon for critical temperatures

crit=70 # critical temperature

read -r temp </sys/class/thermal/thermal_zone0/temp
temp=${temp%???}

if [ "$temp" -lt "$crit" ] ; then
    printf "$ICONn%s°C" "$temp"
else
    printf "$ICONc%s°C" "$temp"
fi
