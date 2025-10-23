### 主要变化总结

头文件：#include <boost/variant.hpp>

类型名：std::variant → boost::variant

异常类型：std::bad_variant_access → boost::bad_get

索引查询：v.index() → v.which()

类型大小：std::variant_size_v → boost::variant_size<...>::value

空状态：std::monostate → boost::blank

访问模式：std::visit → boost::apply_visitor + 继承 boost::static_visitor 的访问者类

获取值：std::get → boost::get（注意：boost::variant 没有基于索引的 get）

注意：boost::variant 没有直接的 std::holds_alternative 等价物，但可以通过比较 which() 的结果来实现类似功能。
