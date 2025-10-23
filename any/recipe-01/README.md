### 主要变化：

头文件：#include <boost/any.hpp> 替换 #include <any>

命名空间：所有 std::any 替换为 boost::any

方法调用：

has_value() → empty()（注意逻辑取反）

std::any_cast → boost::any_cast

std::bad_any_cast → boost::bad_any_cast

重置值：reset() → = boost::any()（因为 Boost.Any 没有 reset 方法）

注意：empty() 方法与 has_value() 逻辑相反，empty() 返回 true 表示没有值，而 has_value() 返回 true 表示有值。
