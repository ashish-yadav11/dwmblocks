#!/bin/sh

ICONc="" # icon for critical temperatures
ICONn="" # icon for normal temperatures

crit=70 # critical temperature

read -r temp </sys/class/thermal/thermal_zone0/temp
temp=${temp%???}

if [ "$temp" -ge "$crit" ] ; then
    echo "${ICONc}${temp}°C"
else
    echo "${ICONn}${temp}°C"
fi
