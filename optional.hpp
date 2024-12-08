#pragma once
#include<type_traits>
namespace cppp{
    template<typename T>
    class optional{
        bool has_value;
        alignas(T) unsigned char data[sizeof(T)];
        optional(bool) : has_value(true){}
        void _destroy(){
            if(has_value){
                (**this).~T();
            }
        }
        public:
            optional() noexcept : has_value(false){}
            optional(const optional& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
            requires(std::is_copy_constructible_v<T>) : has_value(true){
                emplace(*other);
            }
            optional(optional&& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
            requires(std::is_copy_constructible_v<T>) : has_value(true){
                emplace(std::move(*other));
            }
            optional& operator=(const optional& other)
            noexcept(std::is_nothrow_copy_assignable_v<T>)
            requires(std::is_copy_assignable_v<T>){
                **this = *other;
                return *this;
            }
            optional& operator=(optional&& other)
            noexcept(std::is_nothrow_move_assignable_v<T>)
            requires(std::is_move_assignable_v<T>){
                **this = std::move(*other);
                return *this;
            }
            optional(optional&& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
            requires(std::is_copy_constructible_v<T>) : has_value(true){
                emplace(std::move(*other));
            }
            template<typename ...A>
            void emplace(A&& ...a) noexcept(std::is_nothrow_constructible_v<T,A...>){
                _destroy();
                new(data) T(std::forward<A>(a)...);
                has_value = true;
            }
            void destroy(){
                _destroy();
                has_value = true;
            }
            template<typename ...A>
            static optional construct(A&& ...a) noexcept(std::is_nothrow_constructible_v<T,A...>){
                optional<T> v{true};
                v.emplace<A...>(std::forward<A>(a)...);
                return v;
            }
            T* operator->() noexcept{
                return reinterpret_cast<T*>(data);
            }
            const T* operator->() const noexcept{
                return reinterpret_cast<const T*>(data);
            }
            T& operator*() noexcept{
                return *reinterpret_cast<T*>(data);
            }
            const T& operator*() const noexcept{
                return *reinterpret_cast<const T*>(data);
            }
            T* ptr(){
                return has_value?reinterpret_cast<T*>(data):nullptr;
            }
            const T* ptr() const{
                return has_value?reinterpret_cast<const T*>(data):nullptr;
            }
            explicit operator bool() const noexcept{
                return has_value;
            }
            ~optional(){
                _destroy();
            }
    };
}
