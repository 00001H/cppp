#pragma once
#include<type_traits>
#include<utility>
namespace cppp{
    template<typename T,bool copyable=false> requires(std::is_pointer_v<T>||std::is_arithmetic_v<T>)
    class zeroing_field{
        T value;
        public:
            constexpr zeroing_field() noexcept : value(/*zero*/){}
            constexpr zeroing_field(T value) noexcept : value(value){}
            constexpr zeroing_field(const zeroing_field& other) noexcept requires(copyable) : value(other.value){}
            constexpr zeroing_field(zeroing_field&& other) noexcept : value(std::exchange(other.value,T())){}
            constexpr zeroing_field& operator=(T newv) noexcept{
                value = newv;
                return *this;
            }
            constexpr zeroing_field& operator=(const zeroing_field& other) noexcept requires(copyable){
                value = other.value;
                return *this;
            }
            constexpr zeroing_field& operator=(zeroing_field&& other) noexcept{
                value = std::exchange(other.value,T());
                return *this;
            }
            constexpr void reset() noexcept{
                value = T();
            }
            constexpr T& operator*() noexcept{
                return value;
            }
            constexpr const T& operator*() const noexcept{
                return value;
            }
    };
}
