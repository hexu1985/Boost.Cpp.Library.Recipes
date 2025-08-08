#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./echo_selectserv 9999 &
SERVER_PID=$!

PIDARRAY=()

for i in $(seq 1 7)
do
    ./echo_client 127.0.0.1 9999 <<EOF &
abc
123
456
789
110
Q
EOF
PIDARRAY+=("$!")
done

wait ${PIDARRAY[@]}

kill -9 ${SERVER_PID}

echo ""
