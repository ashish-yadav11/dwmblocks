#!/bin/sh
# (for pipewire users)
# This script parses the output of `pactl list sinks' to find volume and mute
# status of the default audio sink and whether headphones are plugged in or not
# Also see ../daemons/pulse_daemon.sh
sink="$(pactl info | awk '$1 == "Default" && $2 == "Sink:" {print $3}')"
[ -n "$sink" ] || exit
pactl list sinks | awk -v sink="$sink" '
    BEGIN {
        ICONsn = "\x0c\x0b" # headphone unplugged, not muted
        ICONsm = "\x0d\x0b" # headphone unplugged, muted
        ICONhn = "\x0c\x0b" # headphone plugged in, not muted
        ICONhm = "\x0d\x0b" # headphone plugged in, muted
    }
    f {
        if ($1 == "Mute:" && $2 == "yes") {
            m = 1
        } else if ($1 == "Volume:") {
            if ($3 == $10) {
                vb = $5
            } else {
                vl = $5
                vr = $12
            }
        } else if ($1 == "Active" && $2 == "Port:") {
            if (tolower($3) ~ /headphone/)
                h = 1
            exit
        }
        next
    }
    $1 == "Name:" && $2 == sink {
        f = 1
    }
    END {
        if (f) {
            printf "%s", h ? (m ? ICONhm : ICONhn) : (m ? ICONsm : ICONsn)
            if (vb)
                print vb
            else
                printf "L%s R%s\n", vl, vr
        }
    }
'
