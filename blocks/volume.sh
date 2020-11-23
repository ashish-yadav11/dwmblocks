#!/bin/sh
ICONhm="" # headphone plugged in, muted
ICONhn="" # headphone plugged in, not muted
ICONsm="" # headphone unplugged, muted
ICONsn="" # headphone unplugged, not muted

pacmd list-sinks |
    awk -v iconhm="$ICONhm" -v iconhn="$ICONhn" -v iconsm="$ICONsm" -v iconsn="$ICONsn" '
        /\* index: /,0 {
            if ($1 == "index:") {
                exit
            } else if ($1 == "muted:" && $2 == "yes") {
                muted = 1
            } else if ($1 == "volume:") {
                volumel = $3
                volumer = $10
                volumelp = $5
                volumerp = $12
            } else if ($1 == "active" && $2 == "port:" && $3 ~ /headphones/) {
                headphone = 1
            }
        }
        END {
            printf "%s", headphone ? (muted ? iconhm : iconhn) : (muted ? iconsm : iconsn)
            if (volumel == volumer) {
                print volumelp
            } else {
                printf "L%s R%s\n", volumelp, volumerp
            }
        }
    '
