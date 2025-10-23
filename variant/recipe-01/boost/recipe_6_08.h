#pragma once

#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <iomanip>
#include <chrono>

namespace recipe_6_08
{
   using namespace std::string_literals;

   struct foo
   {
      int value;

      explicit foo(int const i) : value(i) {}
   };

   void execute()
   {
      {
         boost::variant<int, double, std::string> v;
         v = 42;   // v contains int 42
         v = 42.0; // v contains double 42.0
         v = "42"; // v contains string "42"
      }

      {
         boost::variant<int, double, std::string> v = 42;

         auto i1 = boost::get<int>(v);
         auto i2 = boost::get<int>(v); // 注意：boost::variant 没有基于索引的get

         try
         {
            auto f = boost::get<double>(v);
         }
         catch (boost::bad_get const & e)
         {
            std::cout << e.what() << '\n'; // Unexpected index
         }
      }

      {
         boost::variant<int, double, std::string> v = 42;
         // boost::variant 没有直接的 variant_size
         // static_assert(boost::variant_size<decltype(v)>::value == 3);
         std::cout << "index = " << v.which() << '\n'; // 注意：使用 which() 而不是 index()
         v = 42.0;
         std::cout << "index = " << v.which() << '\n';
         v = "42";
         std::cout << "index = " << v.which() << '\n';
      }

      {
         // boost::variant 没有直接的 holds_alternative，但可以使用 which() 实现类似功能
         boost::variant<int, double, std::string> v = 42;
         std::cout << "int? " << std::boolalpha << (v.which() == 0) << '\n'; // int? true

         v = "42";
         std::cout << "int? " << std::boolalpha << (v.which() == 0) << '\n'; // int? false
      }

      {
         boost::variant<int, std::string, foo> v = 42;
         // boost::variant 没有 emplace，但可以直接赋值
         v = foo(42);
      }

      {
         //boost::variant<foo, int> v; // error

         // boost::variant 使用 boost::blank 代替 std::monostate
         boost::variant<boost::blank, foo, int> v; 
         v = 42;       // v contains int 42
         std::cout << boost::get<int>(v) << '\n';
         v = foo(42); // v contains foo{42}
         std::cout << boost::get<foo>(v).value << '\n';
      }

      {
         boost::variant<int, double, std::string> v = 42;
         boost::apply_visitor([](auto&& arg) {std::cout << arg << '\n'; }, v);
         v = 42.0;
         boost::apply_visitor([](auto&& arg) {std::cout << arg << '\n'; }, v);

#ifdef CONSTEXPR_IF_AVAILABLE
         boost::apply_visitor([](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) std::cout << "int " << arg << '\n';
            else if constexpr (std::is_same_v<T, double>) std::cout << "double " << arg << '\n';
            else if constexpr (std::is_same_v<T, std::string>) std::cout << "std::string " << arg << '\n';
         }, v);
#endif
      }
   }
}
