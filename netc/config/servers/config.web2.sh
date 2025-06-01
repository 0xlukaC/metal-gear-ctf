#!/bin/sh
set -eux
sleep 1 # race condition, this will run before switch
# apk add nginx

ip addr add 10.10.10.12/24 dev eth1
ip link set eth1 up
apk add gcc musl-dev iproute2
gcc /config/web1.c -o /config/exec
ls /config
chmod +x /config/exec
/config/exec


# # tail -f /dev/null
