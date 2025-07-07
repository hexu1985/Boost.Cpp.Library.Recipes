# Boost.Asio system_timer 的线程安全性

Boost.Asio 中的 `system_timer` 类（以及其它 I/O 对象）的线程安全性遵循特定的规则，理解这些规则对于编写正确的多线程程序至关重要。

## 基本线程安全规则

1. **对象本身非线程安全**：
   - `system_timer` 对象本身不是线程安全的
   - 不能从多个线程同时调用同一个 `system_timer` 对象的成员函数

2. **并发操作限制**：
   - 不能同时在多个线程上对同一个定时器执行 `async_wait()`
   - 在一个异步等待操作完成之前，不能启动另一个异步等待操作

## 安全使用模式

### 1. 单线程使用（最常见模式）

```cpp
boost::asio::io_context io;
boost::asio::system_timer timer(io);

// 所有timer操作都在同一个线程中执行
timer.expires_after(std::chrono::seconds(1));
timer.async_wait([](...){...});

io.run(); // 在单一线程中运行
```

### 2. 多线程安全访问

如果需要从多个线程访问定时器，必须通过 `io_context` 的 `post()` 或 `dispatch()` 方法：

```cpp
void set_timer(boost::asio::system_timer& timer, 
               std::chrono::seconds sec,
               boost::asio::io_context& io) {
    io.post([&timer, sec]() {
        timer.expires_after(sec);
        timer.async_wait([](const boost::system::error_code& ec) {
            if (!ec) std::cout << "Timer expired\n";
        });
    });
}
```

### 3. 多线程运行 io_context

可以安全地在多个线程中运行 `io_context`，但定时器操作仍需通过 `post/dispatch` 或限制在单个线程中：

```cpp
boost::asio::io_context io;
boost::asio::system_timer timer(io);

// 在两个线程中运行io_context
std::thread t1([&io](){ io.run(); });
std::thread t2([&io](){ io.run(); });

// 安全设置定时器
io.post([&timer](){
    timer.expires_after(std::chrono::seconds(2));
    timer.async_wait([](...){...});
});

t1.join();
t2.join();
```

## 常见线程安全问题及解决方案

### 问题1：竞态条件

**不安全代码**：
```cpp
// 线程1
timer.expires_after(sec1);
timer.async_wait(handler1);

// 线程2
timer.expires_after(sec2);
timer.async_wait(handler2);
```

**解决方案**：通过 `io_context` 序列化访问

### 问题2：取消定时器的线程安全问题

**不安全代码**：
```cpp
// 线程1
timer.cancel();

// 线程2
timer.async_wait(handler);
```

**解决方案**：
```cpp
io.post([&timer](){ timer.cancel(); });
```

## 最佳实践

1. **尽量在单一线程中操作定时器**
2. **必须多线程访问时，使用 `post()` 或 `dispatch()`**
3. **避免在回调函数中直接操作定时器（除非在同一个线程）**
4. **考虑使用 `strand` 来简化线程安全代码**

### 使用 strand 的示例

```cpp
boost::asio::io_context io;
auto strand = boost::asio::make_strand(io);
boost::asio::system_timer timer(io);

// 线程安全地使用定时器
boost::asio::post(strand, [&timer](){
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait(boost::asio::bind_executor(strand, 
        [](const boost::system::error_code& ec) {
            if (!ec) std::cout << "Timer expired\n";
        }));
});

// 多线程运行
std::thread t1([&io](){ io.run(); });
std::thread t2([&io](){ io.run(); });
t1.join();
t2.join();
```

## 总结

- `system_timer` 本身不是线程安全的
- 通过 `io_context` 的 `post/dispatch` 或 `strand` 可以实现线程安全访问
- 在多线程环境中运行 `io_context` 是安全的，但需要正确同步对定时器的操作
- 最简单的线程安全模式是在单一线程中使用定时器
