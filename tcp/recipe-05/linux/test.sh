#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./op_server 9999 &


for i in $(seq 1 6)
do
    sleep 1
    ./op_client 127.0.0.1 9999 <<EOF
3 
12 
24 
36
+
EOF
done

echo ""
