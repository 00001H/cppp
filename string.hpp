#pragma once
#include<string>
namespace cppp{
    using str = std::u8string;
    using sv = std::u8string_view;
    inline std::string_view cview(sv x){
        return {reinterpret_cast<const char*>(x.data()),x.length()};
    }
    inline sv uview(std::string_view x){
        return {reinterpret_cast<const char8_t*>(x.data()),x.length()};
    }
}
