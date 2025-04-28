#pragma once
#include<type_traits>
#include<utility>
namespace cppp{
    template<typename T>
    struct default_zero{};
    template<typename T> requires(std::is_pointer_v<T>)
    struct default_zero<T>{
        constexpr static T value{nullptr};
    };
    template<typename T> requires(std::is_integral_v<T>)
    struct default_zero<T>{
        constexpr static T value{0};
    };
    template<typename T,T zero_value=default_zero<T>::value,bool copyable=false> requires(std::is_copy_constructible_v<T>)
    class zeroing_field{
        T value;
        public:
            zeroing_field() : value(zero_value){}
            zeroing_field(T value) : value(value){}
            zeroing_field(const zeroing_field& other) requires(copyable) : value(other.value){}
            zeroing_field(zeroing_field&& other) : value(std::exchange(other.value,zero_value)){}
            zeroing_field& operator=(T newv){
                value = newv;
            }
            zeroing_field& operator=(const zeroing_field& other) requires(copyable){
                value = other.value;
            }
            zeroing_field& operator=(zeroing_field&& other){
                value = std::exchange(other.value,zero_value);
            }
            T& operator*(){
                return value;
            }
            const T& operator*() const{
                return value;
            }
            T* operator->(){
                return &value;
            }
            const T* operator->() const{
                return &value;
            }
    };
}
