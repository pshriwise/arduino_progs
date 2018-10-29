#!/bin/bash

# default port upload
PORT="/dev/ttyACM0"

# help option
if [[ $1 == "-h" || $1 == "--help" ]]; then
    echo "Script for uploading arduino programs"
    echo "Usage: upload.sh <prog_dir> [--port <port> default:/dev/ttyACM0]"
    exit
fi

if [[ $# -gt 1 ]]; then
    PORT="$2"
    echo "Setting port to non-default: $PORT"
fi

# expand directory into program name
PROG="$1/$1.ino"

echo $PATH


# must add self to 'dialout' group to run w/o sudo
arduino --upload $PROG --port $PORT
