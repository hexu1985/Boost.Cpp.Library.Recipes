#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./echo_mpserv 9999 &
SERVER_PID=$!

for i in $(seq 1 3)
do
    sleep 1
    ./echo_mpclient 127.0.0.1 9999 <<EOF
abc
123
456
Q
EOF
done

kill -9 ${SERVER_PID}

echo ""
