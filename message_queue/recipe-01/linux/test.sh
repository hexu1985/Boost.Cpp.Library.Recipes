#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

# test pmsg_create
echo '-------------------------------------'
echo '-------------pmsg_create-------------'
echo '-------------------------------------'
set -x
rm -f /dev/mqueue/mq
./pmsg_create -c /mq
./pmsg_getattr /mq
ls -tlr /dev/mqueue/
./pmsg_unlink /mq
set +x
echo

# test pmsg_write 
echo '-------------------------------------'
echo '-------------pmsg_write--------------'
echo '-------------------------------------'
set -x
./pmsg_create -cx /mq
./pmsg_send /mq msg-a 5
./pmsg_send /mq msg-b 0
./pmsg_send /mq msg-c 10
set +x
echo

# test pmsg_read 
echo '-------------------------------------'
echo '-------------pmsg_read---------------'
echo '-------------------------------------'
set -x
./pmsg_receive /mq
./pmsg_receive /mq
./pmsg_receive /mq

./pmsg_receive /mq &
sleep 3
./pmsg_send /mq msg-d 20

./pmsg_receive -n /mq
set +x
echo

# test mq_notify_sig
echo '-------------------------------------'
echo '-----------mq_notify_sig-------------'
echo '-------------------------------------'
set -x
./mq_notify_sig /mq &
PROG_PID=$!

for i in $(seq 1 3)
do
    ./pmsg_send /mq "msg-$i" 0
    sleep 1
done

kill -9 ${PROG_PID}
set +x
echo

# test mq_notify_thread
echo '-------------------------------------'
echo '----------mq_notify_thread-----------'
echo '-------------------------------------'
set -x
./mq_notify_thread /mq &
PROG_PID=$!

for i in $(seq 1 3)
do
    ./pmsg_send /mq "msg-$i" 0
    sleep 1
done

kill -9 ${PROG_PID}
set +x
echo

# test mq_notify_sigwaitinfo
echo '-------------------------------------'
echo '-------mq_notify_sigwaitinfo---------'
echo '-------------------------------------'
set -x
./mq_notify_sigwaitinfo /mq &
PROG_PID=$!

for i in $(seq 1 3)
do
    ./pmsg_send /mq "msg-$i" 0
    sleep 1
done

kill -9 ${PROG_PID}
set +x
echo

# test mq_notify_via_signal
echo '-------------------------------------'
echo '-------mq_notify_via_signal---------'
echo '-------------------------------------'
set -x
./mq_notify_via_signal /mq &
PROG_PID=$!

for i in $(seq 1 3)
do
    ./pmsg_send /mq "msg-$i" 0
    sleep 2
done

kill -9 ${PROG_PID}
set +x
echo

# test mq_notify_via_thread
echo '-------------------------------------'
echo '-------mq_notify_via_thread---------'
echo '-------------------------------------'
set -x
./mq_notify_via_thread /mq &
PROG_PID=$!

for i in $(seq 1 3)
do
    ./pmsg_send /mq "msg-$i" 0
    sleep 2
done

kill -9 ${PROG_PID}
set +x
echo

