#pragma once
#include<algorithm>
#include<string>
namespace cppp{
    using str = std::u8string;
    using sv = std::u8string_view;
    inline std::string_view cview(sv x){
        return {reinterpret_cast<const char*>(x.data()),x.size()};
    }
    inline sv uview(std::string_view x){
        return {reinterpret_cast<const char8_t*>(x.data()),x.size()};
    }
    constexpr inline sv uview(const std::string&) = delete;
    inline str tou8(const std::string& s){
        return {s.begin(),s.end()};
    }
}
