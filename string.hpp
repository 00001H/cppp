#pragma once
#include<string>
namespace cppp{
    using str = std::u8string;
    using sv = std::u8string_view;
    constexpr inline std::string_view cview(sv x){
        return {reinterpret_cast<const char*>(x.data()),x.length()};
    }
    constexpr inline sv uview(std::string_view x){
        return {reinterpret_cast<const char8_t*>(x.data()),x.length()};
    }
}
