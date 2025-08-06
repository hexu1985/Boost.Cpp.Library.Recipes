#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./echo_mpserv2 9999 &
SERVER_PID=$!

PIDARRAY=()

for i in $(seq 1 3)
do
    ./echo_mpclient 127.0.0.1 9999 <<EOF &
abc
123
456
Q
EOF
PIDARRAY+=("$!")
done

wait ${PIDARRAY[@]}

kill -9 ${SERVER_PID}

echo ""
