#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

sudo route add -host 224.1.1.2 dev wlp0s20f3

./news_receiver 224.1.1.2 9999 &
SERVER_PID=$!

sleep 1

./news_sender 224.1.1.2 9999

kill -9 ${SERVER_PID}

echo ""
