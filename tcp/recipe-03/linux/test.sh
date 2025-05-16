#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./echo_server 9999 &


for i in $(seq 1 6)
do
    sleep 1
    ./echo_client 127.0.0.1 9999 <<EOF
abc
123
456
Q
EOF
done
