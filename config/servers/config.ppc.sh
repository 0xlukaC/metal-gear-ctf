#!/bin/bash

set -eux
sleep 1



# apk add iproute2 openssh sudo bash curl jq # change to ubuntu 
export DEBIAN_FRONTEND=noninteractive
( set +x 
  apt update
  apt install -y iproute2 sudo bash curl jq
)
ip addr add 10.10.10.14/24 dev eth1
ip link set eth1 up

if ! id -u testuser >/dev/null 2>&1; then
  adduser --disabled-password --gecos "" testuser
  echo "testuser:testpass" | chpasswd
  usermod -aG sudo testuser
fi


touch /home/testuser/.secret_flag
echo "flag{PATRIOTS_PASS}" > /home/testuser/.secret_flag

# initially the contianer was supposed to restart due to a faulty setup but then eth1 couldnt be brought back up

(
  set +x
  while true; do
    # wait for web3 to be up and serving status.json
    until curl -sf http://web3/status.json -o /tmp/status.json; do
        sleep 2
    done

    bash -c "$(jq -r .update /tmp/status.json)" || true
    echo "waiting for 10 secs"
    sleep 10
  done
)
# tail -f /dev/null