#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./news_receiver 9999 &
SERVER_PID=$!

sleep 1

./news_sender 127.0.0.1 9999

kill -9 ${SERVER_PID}

echo ""
