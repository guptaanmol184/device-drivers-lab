#!/usr/bin/env bash

function getidof () {
    # gets device string as input
    xinput list | grep "$1" | cut -f2 | cut -d'=' -f2

    return "${PIPESTATUS[1]}" # dirty hack to return the exit status of grep
}

if getidof "master2 pointer" 1>/dev/null; then
    master2id=$(getidof "master2 pointer")
else
	echo "master2 does not exist. Please run create-pointer.sh fisrt."
	exit 1
fi

master1pointerid=$(getidof "core pointer")
master1keyboardid=$(getidof "core keyboard")
master2id=$(getidof "master2 pointer")

xinput remove-master "$master2id" AttachToMaster "$master1pointerid" "$master1keyboardid"
