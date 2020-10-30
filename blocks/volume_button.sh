#!/bin/sh
case $1 in
    1) pamixer --toggle-mute ;;
    2) exec pavucontrol-qt ;;
    3) pamixer --set-volume 60 ;;
esac
