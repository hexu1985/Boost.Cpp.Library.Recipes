#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <boost/optional.hpp>

namespace recipe_6_07
{
   using namespace std::string_literals;

   struct foo
   {
      int    a;
      double b;
   };

   struct bar
   {
      bar() { std::cout << "default ctor" << '\n'; }
      ~bar() { std::cout << "dtor" << '\n'; }
      bar(bar const& b) { std::cout << "copy ctor" << '\n'; }
      bar(bar&& b) { std::cout << "move ctor" << '\n'; }
   };

   template <typename K, typename V>
   boost::optional<V> find(int const key, std::map<K, V> const & m)
   {
      auto pos = m.find(key);
      if (pos != m.end())
         return pos->second;

      return boost::none;
   }

   struct book
   {
      std::string                 title;
      boost::optional<std::string>  subtitle;
      std::vector<std::string>    authors;
      std::string                 publisher;
      std::string                 isbn;
      boost::optional<int>          pages;
      boost::optional<int>          year;
   };

   std::string extract(std::string const & text, 
                       boost::optional<int> start, 
                       boost::optional<int> end)
   {
      auto s = start.get_value_or(0);
      auto e = end.get_value_or(text.length());
      return text.substr(s, e - s);
   }

   void process(boost::optional<bar> const & arg)
   {
      /* do something with arg */
   }

   void execute()
   {
      auto lprint = [](auto const & o)
      {
         if (o) std::cout << *o << '\n';
         else std::cout << "(empty)" << '\n';
      };

      {
         boost::optional<int> v1;
         lprint(v1); // (empty)
         boost::optional<int> v2(42);
         lprint(v2); // 42
         v1 = 42;
         lprint(v1); // 42
         boost::optional<int> v3 = v2;
         lprint(v3); // 42
      }

      {
         boost::optional<int> v1{ 42 };
         std::cout << *v1 << '\n';   // 42

         boost::optional<foo> v2{ foo{ 42, 10.5 } };
         std::cout << v2->a << ", " 
                   << v2->b << '\n'; // 42, 10.5
      }

      {
         boost::optional<int> v1{ 42 };
         if (v1) std::cout << *v1 << '\n';

         boost::optional<foo> v2{ foo{ 42, 10.5 } };
         if (v2.is_initialized()) std::cout << v2->a << ", " << v2->b << '\n';
      }

      {
         boost::optional<std::string> v1{ "text"s };
         std::cout << v1.get() << '\n';  // text

         boost::optional<std::string> v2;
         std::cout << v2.get_value_or("default"s) 
                   << '\n';                // default
      }

      {
         boost::optional<int> v{ 42 };
         lprint(v); // 42
         v = boost::none;
         lprint(v); // (empty)
      }

      {
         std::map<int, std::string> m{ { 1, "one"s },{ 2, "two"s },{ 3, "three"s } };
         auto value = find(2, m);
         if (value) std::cout << *value << '\n'; // two

         value = find(4, m);
         if (value) std::cout << *value << '\n';
      }

      {
         auto v1 = extract("sample"s, boost::none, boost::none);
         std::cout << v1 << '\n';   // sample

         auto v2 = extract("sample"s, 1, boost::none);
         std::cout << v2 << '\n';   // ample

         auto v3 = extract("sample"s, 1, 4);
         std::cout << v3 << '\n';   // amp
      }

      {
         boost::optional<bar> b1{ bar{} };
         bar b2{};

         process(b1);  // no copy
         process(b2);  // copy construction
      }
   }
}
