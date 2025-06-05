#!/bin/sh
set -eux
sleep 1 # due to race condition, this will run before switch

ip addr add 10.10.10.13/24 dev eth1
ip link set eth1 up
apk add -q gcc musl-dev iproute2
gcc /config/web1.c -o /config/exec
ls /config
chmod +x /config/exec
/config/exec

# # tail -f /dev/null
