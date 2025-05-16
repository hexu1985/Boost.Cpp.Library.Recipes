#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./tcp_server 9999 &

sleep 1

./tcp_client 127.0.0.1 9999
