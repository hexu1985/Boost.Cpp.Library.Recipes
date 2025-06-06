#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

sudo route add -host 224.1.1.2 dev wlp0s20f3

declare -a receiver_pids

./news_receiver 224.1.1.2 9999 &
receiver_pids+=($!)

./news_receiver 224.1.1.2 9999 &
receiver_pids+=($!)

sleep 1

./news_sender 224.1.1.2 9999

for pid in "${receiver_pids[@]}"
do
    echo $pid
    kill -9 ${pid}
done

echo ""
