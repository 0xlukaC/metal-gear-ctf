#!/bin/sh
set -eux
sleep 1 # due to race condition, this will run before switch

ip addr add 10.10.10.13/24 dev eth1
ip link set eth1 up
apk add -q gcc musl-dev iproute2
# apk add openssh sudo iproute2 bash

# if ! id -u testuser >/dev/null 2>&1; then
#   adduser -D testuser
#   echo "testuser:testpass" | chpasswd
#   addgroup testuser wheel
# fi


touch /frontend/access.log

real_msg=$(echo -n "Secret: try adding /top to /secret/password" | base64)

real_line=$((RANDOM % 1000 + 1))

for i in $(seq 1 1000); do
    ip="10.10.10.$((RANDOM % 254 + 1))"
    method="GET"
    if [ "$i" -eq "$real_line" ]; then
        echo "$ip - $method /api/data?msg=$real_msg" >> /frontend/access.log
    else
        junk=$(head /dev/urandom | tr -dc 'A-Za-z0-9' | head -c $((RANDOM % 20 + 10)) | base64)
        echo "$ip - $method /api/data?msg=$junk" >> /frontend/access.log
    fi
done

#  grep -oP 'msg=\K[A-Za-z0-9+/=]+' access.log | strings | base64 -d | grep "Secret"

gcc /config/web1.c -o /config/exec
ls /config
chmod +x /config/exec
/config/exec

# # tail -f /dev/null
