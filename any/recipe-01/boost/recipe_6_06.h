#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <boost/any.hpp>  // 替换 <any> 为 <boost/any.hpp>

namespace recipe_6_06
{
   using namespace std::string_literals;

   void log(boost::any const & value)  // 替换 std::any 为 boost::any
   {
      if (!value.empty())  // 替换 has_value() 为 empty()
      {
         auto const & tv = value.type();
         if (tv == typeid(int))
         {
            std::cout << boost::any_cast<int>(value) << '\n';  // 替换 std::any_cast
         }
         else if (tv == typeid(std::string))
         {
            std::cout << boost::any_cast<std::string>(value) << '\n';  // 替换 std::any_cast
         }
         else if (tv == typeid(std::chrono::time_point<std::chrono::system_clock>))
         {
            auto t = boost::any_cast<std::chrono::time_point<std::chrono::system_clock>>(value);  // 替换 std::any_cast
            auto now = std::chrono::system_clock::to_time_t(t);
            std::cout << std::put_time(std::localtime(&now), "%F %T") << '\n';
         }
         else
         {
            std::cout << "unexpected value type" << '\n';
         }
      }
      else
      {
         std::cout << "(empty)" << '\n';
      }
   }

   inline bool is_integer(boost::any const & a)  // 替换 std::any 为 boost::any
   {
      return a.type() == typeid(int);
   }

   void execute()
   {
      {
         boost::any value(42);  // 替换 std::any 为 boost::any
         value = 42.0;        // double      42.0
         value = "42"s;       // std::string "42"
      }

      {
         auto ltest = [](boost::any const & a) {  // 替换 std::any 为 boost::any
            if (!a.empty())  // 替换 has_value() 为 empty()
               std::cout << "has value" << '\n';
            else
               std::cout << "no value" << '\n';
         };

         boost::any value;  // 替换 std::any 为 boost::any
         ltest(value);   // no value
         value = 42;
         ltest(value);   // has value
         value = boost::any();  // 替换 reset() 为赋空值
         ltest(value);   // no value
      }

      {
         boost::any value = 42.0;  // 替换 std::any 为 boost::any

         try
         {
            auto d = boost::any_cast<double>(value);  // 替换 std::any_cast
            std::cout << d << '\n';
         }
         catch (boost::bad_any_cast const & e)  // 替换 std::bad_any_cast 为 boost::bad_any_cast
         {
            std::cout << e.what() << '\n';
         }

         try
         {
            auto pd = boost::any_cast<double>(&value);
            std::cout << *pd << '\n';
         }
         catch (boost::bad_any_cast const & e)
         {
            std::cout << e.what() << '\n';
         }

         try
         {
            auto i = boost::any_cast<int>(value);  // 替换 std::any_cast
            std::cout << i << '\n';
         }
         catch (boost::bad_any_cast const & e)  // 替换 std::bad_any_cast 为 boost::bad_any_cast
         {
            std::cout << e.what() << '\n';
         }

         value = "sample"s;
         try
         {
            auto s = boost::any_cast<std::string>(value);  // 替换 std::any_cast
            std::cout << s << '\n';
         }
         catch (boost::bad_any_cast const & e)  // 替换 std::bad_any_cast 为 boost::bad_any_cast
         {
            std::cout << e.what() << '\n';
         }

         value = std::vector<int>{ 1, 1, 2, 3, 5, 8 };
         try
         {
            auto v = boost::any_cast<std::vector<int>>(value);  // 替换 std::any_cast
            for (auto e : v)
               std::cout << e << '\n';
         }
         catch (boost::bad_any_cast const & e)  // 替换 std::bad_any_cast 为 boost::bad_any_cast
         {
            std::cout << e.what() << '\n';
         }
      }

      {
         log(boost::any{});  // 替换 std::any 为 boost::any
         log(42);        // 42
         log("42"s);     // 42
         log(42.0);      // unexpected value type
         log(std::chrono::system_clock::now());  // 2016-10-30 22:42:57
      }

      {
         std::vector<boost::any> values;  // 替换 std::any 为 boost::any
         values.push_back(boost::any{});  // 替换 std::any 为 boost::any
         values.push_back(42);
         values.push_back("42"s);
         values.push_back(42.0);
         values.push_back(std::chrono::system_clock::now());

         for (auto const v : values)
            log(v);
      }

      {
         struct foo
         {
            foo() = default;
            foo(foo const&) = delete;
         };

         [[maybe_unused]] foo f;
         // boost::any value = f; // error - 同样不支持不可拷贝类型
      }
   }
}
