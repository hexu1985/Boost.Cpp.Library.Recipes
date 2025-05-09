#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

# test pshm_create
echo '-------------------------------------'
echo '-------------pshm_create-------------'
echo '-------------------------------------'
set -x
rm -f /dev/shm/demo_shm
ls -l /dev/shm
./pshm_create -c /demo_shm 10000
ls -l /dev/shm
set +x
echo

# test pshm_write 
echo '-------------------------------------'
echo '-------------pshm_write--------------'
echo '-------------------------------------'
set -x
ls -l /dev/shm
./pshm_write /demo_shm 'hello'
ls -l /dev/shm
set +x
echo

# test pshm_read 
echo '-------------------------------------'
echo '-------------pshm_read---------------'
echo '-------------------------------------'
set -x
ls -l /dev/shm
./pshm_read /demo_shm
ls -l /dev/shm
set +x
echo

# test pshm_unlink 
echo '-------------------------------------'
echo '-------------pshm_unlink-------------'
echo '-------------------------------------'
set -x
ls -l /dev/shm
./pshm_unlink /demo_shm
ls -l /dev/shm
set +x
echo
