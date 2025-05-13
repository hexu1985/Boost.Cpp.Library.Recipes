#!/usr/bin/env python3

import threading
import datetime

def print_message(message):
    print(f"{datetime.datetime.now()}: {message}")

def hello():
    print_message("Hello, World!")

# 创建一个定时器，5秒后执行hello函数
timer = threading.Timer(5.0, hello)
timer.start()  # 启动定时器

print_message("定时器已启动，等待5秒...")
