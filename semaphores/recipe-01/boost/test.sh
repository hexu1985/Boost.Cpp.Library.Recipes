#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

# test psem_create
echo '-------------------------------------'
echo '-------------psem_create-------------'
echo '-------------------------------------'
set -x
umask 007
rm -f /dev/shm/sem.*
ls -l /dev/shm/sem.*
./psem_create -cx /demo 666
ls -l /dev/shm/sem.*
./psem_create -cx /demo 666
set +x
echo

# test named semaphores
echo '-------------------------------------'
echo '-----------named semaphores----------'
echo '-------------------------------------'
set -x
./psem_create -c /demo 600 0
./psem_wait /demo &
sleep 1

#./psem_getvalue /demo

./psem_post /demo

./psem_post /demo
#./psem_getvalue /demo

./psem_unlink /demo
ls -l /dev/shm/sem.*
set +x
echo


