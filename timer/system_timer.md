# C++ Asio 中 system_timer 的状态转换

在 C++ Asio 库中，`system_timer`（属于 Boost.Asio 或 standalone Asio）是一个基于系统时钟的定时器，它的状态转换主要涉及以下几种状态和操作：

## 基本状态

1. **未激活 (Inactive)** - 定时器刚创建时的初始状态
2. **等待中 (Pending)** - 定时器已启动，正在等待到期
3. **已到期 (Expired)** - 定时器已到达设定的时间点
4. **已取消 (Cancelled)** - 定时器被显式取消

## 状态转换图

```
[未激活]
   |
   | async_wait() 或 expires_*() + wait()/async_wait()
   v
[等待中] --- 到期时间到达 ---> [已到期]
   |                             |
   | 调用 cancel()               | 调用 expires_*() 重新设置时间
   v                             v
[已取消]                      [等待中] (如果设置了新的等待)
```

## 主要操作及其影响

1. **构造定时器**
   - 创建后处于 **未激活** 状态

2. **expires_at()/expires_after()**
   - 设置到期时间，但不改变当前状态
   - 需要配合 wait() 或 async_wait() 才会进入 **等待中** 状态

3. **async_wait()**
   - 从 **未激活** 或 **已到期** 转为 **等待中**
   - 如果已在 **等待中**，会取消之前的异步操作并重新开始等待

4. **cancel()**
   - 从 **等待中** 转为 **已取消**
   - 会触发 completion handler 并传递 error::operation_aborted 错误码

5. **自然到期**
   - 从 **等待中** 转为 **已到期**
   - 触发 completion handler 并传递成功错误码

## 重要注意事项

- 一个定时器一次只能有一个异步等待操作 (async_wait) 在进行
- 如果在定时器到期前再次调用 async_wait()，会隐式取消之前的等待
- 定时器状态可以通过 `expiry()` 方法查询下一次到期时间
- 在多线程环境中使用时需要适当的同步措施

## 示例代码

```cpp
asio::system_timer timer(io_context);

// 未激活 -> 等待中
timer.expires_after(std::chrono::seconds(5));
timer.async_wait([](const asio::error_code& ec) {
    if (!ec) {
        // 定时器自然到期
    } else if (ec == asio::error::operation_aborted) {
        // 定时器被取消
    }
});

// 等待中 -> 已取消
timer.cancel();
```

理解这些状态转换对于正确使用 Asio 定时器非常重要，特别是在处理异步操作和错误情况时。
