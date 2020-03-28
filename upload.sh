#!/bin/bash

# default port upload
PORT="/dev/ttyACM0"

# help option
if [[ $1 == "-h" || $1 == "--help" ]]; then
    echo "Script for uploading arduino programs"
    echo "Usage: upload.sh <path_to_program> [--port <port> default:/dev/ttyACM0]"
    echo "Reminder: directory and program must have the same base name."
    exit
fi

if [[ $# -gt 1 ]]; then
    PORT="$2"
    echo "Setting port to non-default: $PORT"
fi

owner=$(stat -c "%U" $PORT)

if [[ "$owner" != "$(whoami)" ]]; then
    printf "Requested port %s is not owned by %s.\n" $PORT $(whoami)
    echo "Enter password to update permissions:"
    sudo chown $(whoami) $PORT
fi

PROG=$1
echo "Uploading program: $PROG"

# must add self to 'dialout' group to run w/o sudo
arduino --upload $PROG --port $PORT
