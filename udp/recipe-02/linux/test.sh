#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./bound_host1 9999 &
SERVER_PID=$!

sleep 1

./bound_host2 127.0.0.1 9999

wait ${SERVER_PID}

echo ""
