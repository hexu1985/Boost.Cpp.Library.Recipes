### 主要修改说明

1. 头文件替换：

```cpp
#include <optional> → #include <boost/optional.hpp>

std::optional → boost::optional
```

2. API 差异处理：

```
value() → get()

value_or() → get_value_or()

has_value() → is_initialized()

reset() → = boost::none

默认构造的空 optional → boost::none
```
