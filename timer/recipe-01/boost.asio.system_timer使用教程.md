# Boost.Asio system_timer 使用教程

`system_timer` 是 Boost.Asio 库中的一个计时器类，用于基于系统时钟的定时操作。它是 `basic_waitable_timer` 模板类的一个特化版本，使用 `std::chrono::system_clock` 作为时钟源。

## 基本用法

### 1. 包含必要头文件

```cpp
#include <boost/asio.hpp>
#include <iostream>
```

### 2. 创建 io_context 和 timer

```cpp
boost::asio::io_context io;
boost::asio::system_timer timer(io);
```

### 3. 设置定时器并等待

```cpp
// 设置3秒后到期
timer.expires_after(std::chrono::seconds(3));

// 异步等待
timer.async_wait([](const boost::system::error_code& ec) {
    if (!ec) {
        std::cout << "Timer expired!" << std::endl;
    }
});

// 运行io_context
io.run();
```

## 完整示例代码

### 示例1：基本定时器使用

```cpp
#include <boost/asio.hpp>
#include <iostream>

int main() {
    boost::asio::io_context io;
    boost::asio::system_timer timer(io);

    // 设置5秒后到期
    timer.expires_after(std::chrono::seconds(5));

    std::cout << "Starting timer, will expire in 5 seconds..." << std::endl;

    timer.async_wait([](const boost::system::error_code& ec) {
        if (!ec) {
            std::cout << "Timer expired at " 
                      << std::chrono::system_clock::now().time_since_epoch().count()
                      << std::endl;
        } else {
            std::cerr << "Timer error: " << ec.message() << std::endl;
        }
    });

    io.run();

    return 0;
}
```

### 示例2：周期性定时器

```cpp
#include <boost/asio.hpp>
#include <iostream>

void print(const boost::system::error_code& /*ec*/,
           boost::asio::system_timer* timer,
           int* count) {
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);

        // 重新设置定时器，1秒后再次触发
        timer->expires_at(timer->expiry() + std::chrono::seconds(1));
        timer->async_wait(std::bind(print, 
                                   std::placeholders::_1, 
                                   timer, 
                                   count));
    }
}

int main() {
    boost::asio::io_context io;
    boost::asio::system_timer timer(io);
    int count = 0;

    // 初始设置1秒后触发
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait(std::bind(print, 
                              std::placeholders::_1, 
                              &timer, 
                              &count));

    io.run();

    std::cout << "Final count is " << count << std::endl;

    return 0;
}
```

### 示例3：取消定时器

```cpp
#include <boost/asio.hpp>
#include <thread>
#include <iostream>

int main() {
    boost::asio::io_context io;
    boost::asio::system_timer timer(io);

    // 设置10秒后到期
    timer.expires_after(std::chrono::seconds(10));

    timer.async_wait([](const boost::system::error_code& ec) {
        if (ec == boost::asio::error::operation_aborted) {
            std::cout << "Timer was cancelled" << std::endl;
        } else if (!ec) {
            std::cout << "Timer expired normally" << std::endl;
        }
    });

    // 在另一个线程中运行io_context
    std::thread t([&io]() { io.run(); });

    // 主线程等待2秒后取消定时器
    std::this_thread::sleep_for(std::chrono::seconds(2));
    timer.cancel();

    t.join();
    return 0;
}
```

## 关键方法说明

1. **expires_after()** - 设置相对时间的到期时间
   ```cpp
   timer.expires_after(std::chrono::seconds(5));
   ```

2. **expires_at()** - 设置绝对时间的到期时间
   ```cpp
   auto time_point = std::chrono::system_clock::now() + std::chrono::seconds(5);
   timer.expires_at(time_point);
   ```

3. **async_wait()** - 异步等待定时器到期
   ```cpp
   timer.async_wait([](const boost::system::error_code& ec) { /*...*/ });
   ```

4. **cancel()** - 取消定时器
   ```cpp
   timer.cancel();
   ```

5. **expiry()** - 获取当前到期时间点
   ```cpp
   auto expiry_time = timer.expiry();
   ```

## 注意事项

1. `system_timer` 使用系统时钟，可能会受到系统时间调整的影响
2. 如果需要不受系统时间调整影响的定时器，考虑使用 `steady_timer`
3. 确保在定时器回调中检查错误码，以处理取消等情况
4. 多个异步操作不能同时等待同一个定时器

希望这些示例和说明能帮助你理解和使用 Boost.Asio 的 `system_timer`。
