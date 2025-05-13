#!/usr/bin/env python3

import threading
import datetime

def print_message(message):
    print(f"{datetime.datetime.now()}: {message}")

def periodic_task():
    print_message("周期性任务执行")
    # 再次设置定时器以实现周期性执行
    threading.Timer(2.0, periodic_task).start()

# 第一次启动定时器
threading.Timer(2.0, periodic_task).start()

print_message("周期性定时器已启动，每2秒执行一次")
# 让程序运行一段时间
threading.Timer(10.0, lambda: print_message("10秒过去了")).start()
