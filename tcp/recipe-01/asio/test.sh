#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./hello_server 9999 &

sleep 1

./hello_client 127.0.0.1 9999
