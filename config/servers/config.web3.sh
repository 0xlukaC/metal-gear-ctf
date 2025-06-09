#!/bin/sh
set -eux
sleep 1 # due to race condition, this will run before switch

ip addr add 10.10.10.13/24 dev eth1
ip link set eth1 up
apk add -q gcc musl-dev iproute2 findutils
apk add openssh sudo bash

if ! id -u testuser >/dev/null 2>&1; then
  adduser -D testuser
  echo "testuser:testpass" | chpasswd
  addgroup testuser wheel
fi

sed -i 's/^#PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config
sed -i 's/^PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config
ssh-keygen -A
mkdir -p /var/run/sshd
# exec /usr/sbin/sshd -D


(
    set +x
    # remove everything in acces.log (without removing the file itself) 
    touch /access.log

    real_msg=$(echo -n "Secret: its a 'top secret password'" | base64)

    real_line=$((RANDOM % 100 + 1))

    for i in $(seq 1 100); do
        ip="10.10.10.$((RANDOM % 254 + 1))"
        method="GET"
        if [ "$i" -eq "$real_line" ]; then
            echo "$ip - $method /api/data?msg=$real_msg" >> /access.log
        else
            junk=$(head /dev/urandom | tr -dc 'A-Za-z0-9' | head -c $((RANDOM % 20 + 10)) | base64)
            echo "$ip - $method /api/data?msg=$junk" >> /access.log
        fi
    done
)

touch /status.json
chmod 666 /status.json
cat > /status.json <<EOF
{
  "update": "echo all good on this end boss",
  "status": "ok"
}
EOF

#  grep -oP 'msg=\K[A-Za-z0-9+/=]+' access.log | strings | base64 -d | grep "Secret"

gcc /config/web1.c -o /config/exec
ls /config
chmod +x /config/exec
/usr/sbin/sshd -D &
/config/exec
# while true; do
#     /config/exec || echo "webserver crashed, restarting..."
#     sleep 1
# done

# # tail -f /dev/null
