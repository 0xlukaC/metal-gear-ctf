#!/bin/bash

set -eux

IP_ADDR="$1"
CIDR_MASK="$2"


apk update && apk add openssh sudo iproute2 bash curl
if ! id -u testuser >/dev/null 2>&1; then
  adduser -D testuser
  echo "testuser:testpass" | chpasswd
  addgroup testuser wheel
fi

sed -i 's/^#PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config
sed -i 's/^PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config

ssh-keygen -A

ip addr add "${IP_ADDR}/${CIDR_MASK}" dev eth1
ip link set eth0 up

mkdir -p /var/run/sshd
exec /usr/sbin/sshd -D

curl -s http://web3/status.json -o /tmp/status.json
bash /tmp/status.json
sleep 15

# This container will then restart because the script will exit and then the container will be forced to restart