#!/usr/bin/env python3

import threading
import datetime

def print_message(message):
    print(f"{datetime.datetime.now()}: {message}")

def greet(name):
    print_message(f"Hello, {name}!")

# 创建一个带参数的定时器，3秒后执行greet函数
timer = threading.Timer(3.0, greet, args=("Alice",))
timer.start()

print_message("带参数的定时器已启动，等待3秒...")
