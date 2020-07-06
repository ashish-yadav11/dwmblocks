#!/bin/dash

volstat=$(pamixer --get-mute --get-volume)

if "${volstat% *}" ; then
    echo "${volstat#* }%"
else
    echo "${volstat#* }%"
fi
