#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./uecho_server 9999 &
SERVER_PID=$!

sleep 1

./uecho_client 127.0.0.1 9999 <<EOF
Hi UDP Server?
Nice to meet you!
Good bye~
q
EOF

./uecho_con_client 127.0.0.1 9999 <<EOF
Hi UDP Server?
Nice to meet you!
Good bye~
q
EOF

kill -9 ${SERVER_PID}

echo ""
