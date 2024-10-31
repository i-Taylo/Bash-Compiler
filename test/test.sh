#!/system/bin/sh

abort() {
    echo "$1"
    exit 1
}

usage() {
    abort "Usage: $0 <message>"
}

[ -z $1 ] && usage

echo $1
