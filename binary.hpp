#pragma once
#include<type_traits>
#include<cstddef>
#include<span>
namespace cppp{
    using buffer = std::span<std::byte>;
    template<std::size_t l>
    using staticbuffer = std::span<std::byte,l>;
    using frozenbuffer = std::span<const std::byte>;
    template<std::size_t l>
    using frozenstaticbuffer = std::span<const std::byte,l>;
    template<typename T>
    staticbuffer<sizeof(T)> memory(T* v){
        return staticbuffer<sizeof(T)>{reinterpret_cast<std::byte*>(v),sizeof(T)};
    }
    template<typename T>
    frozenstaticbuffer<sizeof(T)> memory(const T* v){
        return frozenstaticbuffer<sizeof(T)>{reinterpret_cast<const std::byte*>(v),sizeof(T)};
    }
}