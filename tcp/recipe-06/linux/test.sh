#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./file_server 9999 &
SERVER_PID=$!

sleep 1

./file_client 127.0.0.1 9999

wait ${SERVER_PID}

head receive.dat

echo ""
