#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./tcp_async_echo_server 9999 &
SERVER_PID=$!

PIDARRAY=()

for i in $(seq 1 7)
do
    ./tcp_async_echo_client 127.0.0.1 9999 <<EOF &
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
