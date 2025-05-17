#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

./gethostbyname www.naver.com

echo "============================"

./gethostbyaddr 8.8.8.8

echo ""
