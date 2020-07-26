#!/bin/sh
read -r capacity </sys/class/power_supply/BAT0/capacity
echo "${capacity}%"
