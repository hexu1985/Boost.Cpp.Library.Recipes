#!/usr/bin/env python3

import threading
import datetime
import time

def print_message(message):
    print(f"{datetime.datetime.now()}: {message}")

def task():
    print_message("任务执行了")

# 创建一个10秒后执行的定时器
timer = threading.Timer(10.0, task)
timer.start()

print_message("定时器已启动，你有5秒时间决定是否取消")
time.sleep(5)  # 等待5秒

# 5秒后询问是否取消
user_input = input("要取消定时器吗？(y/n): ")
if user_input.lower() == 'y':
    timer.cancel()
    print_message("定时器已取消")
else:
    print_message("定时器将继续运行")
