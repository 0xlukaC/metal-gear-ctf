#!/bin/sh
set -eux
sleep 1 # due to race condition, this will run before switch

ip addr add 10.10.10.11/24 dev eth1
ip link set eth1 up
apk add gcc musl-dev iproute2
# exec > >(tee /proc/1/fd/1) 2>&1
# exec > /proc/1/fd/1 2>&1
gcc /config/web1.c -o /config/exec
ls /config
chmod +x /config/exec
/config/exec

# # tail -f /dev/null
