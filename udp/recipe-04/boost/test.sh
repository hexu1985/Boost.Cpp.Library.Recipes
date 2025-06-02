#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./news_receiver_brd 9999 &
SERVER_PID=$!

sleep 1

./news_sender_brd 255.255.255.255 9999

kill -9 ${SERVER_PID}

echo ""
