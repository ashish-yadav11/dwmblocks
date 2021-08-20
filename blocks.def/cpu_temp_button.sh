#!/bin/sh
case "$1" in
    1) exec "$TERMINAL" -e htop -s PERCENT_CPU ;;
    2) exec "$TERMINAL" -e htop ;;
    3) exec "$TERMINAL" -e htop -s PERCENT_MEM ;;
esac
