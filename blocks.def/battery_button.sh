#!/bin/sh

read -r rate </sys/class/power_supply/BAT0/current_now
[ "$rate" = 0 ] && notify-send "Battery fully charged" && exit

read -r ac </sys/class/power_supply/AC/online
read -r charge_now </sys/class/power_supply/BAT0/charge_now

if [ "$ac" = 1 ] ; then
    read -r charge_full </sys/class/power_supply/BAT0/charge_full
    val="$(( charge_full-charge_now ))"
else
    val="$charge_now"
fi

hr="$(( val / rate ))"
mn="$(( (val * 60) / rate - hr * 60 ))"

case "$hr" in
    0)
        case "$mn" in
            0) notify-send "Battery fully charged" ;;
            1) notify-send "1 minute remaining" ;;
            *) notify-send "$mn minutes remaining" ;;
        esac
        ;;
    1)
        case "$mn" in
            0) notify-send "1 hour remaining" ;;
            1) notify-send "1 hour, 1 minute remaining" ;;
            *) notify-send "1 hour, $mn minutes remaining" ;;
        esac
        ;;
    *)
        case "$mn" in
            0) notify-send "$hr hours remaining" ;;
            1) notify-send "$hr hours, 1 minute remaining" ;;
            *) notify-send "$hr hours, $mn minutes remaining" ;;
        esac
        ;;
esac
