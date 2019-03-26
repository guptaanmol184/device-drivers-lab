#!/usr/bin/env bash

function getidof () {
    # gets device string as input
    xinput list | grep "$1" | cut -f2 | cut -d'=' -f2

    return "${PIPESTATUS[1]}" # dirty hack to return the exit status of grep
}

if getidof "master2 pointer" 1>/dev/null; then
    echo "master2 already exists. Not creating master2."
    exit 1
else
    xinput create-master "master2"
fi


master2id=$(getidof "master2 pointer")
touchpadid=$(getidof "Synaptics")

xinput reattach "$touchpadid" "$master2id"
