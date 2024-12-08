#pragma once
#include"polyfill/pack-indexing.hpp"
#include"exchange.hpp"
#include<cstdint>
#include<utility>
#include<limits>
namespace cppp{
    template<typename T>
    struct emplace_tag_t{};
    template<typename T>
    constexpr inline emplace_tag_t<T> emplace_tag{};
    namespace detail{
        inline void destroy(void*,std::size_t){}
        template<typename T,typename ...Etc>
        void destroy(void* data,std::size_t index){
            if(index){
                destroy(data,index-1uz);
            }else{
                delete static_cast<T*>(data);
            }
        }
        template<typename T,typename ...Types>
        struct pack_find{};
        template<typename T,typename ...Types>
        struct pack_find<T,T,Types...>{
            constexpr static inline std::size_t index = 0uz;
        };
        template<typename T,typename U1,typename ...Types>
        struct pack_find<T,U1,T,Types...>{
            constexpr static inline std::size_t index = 1uz;
        };
        template<typename T,typename U1,typename U2,typename ...Types>
        struct pack_find<T,U1,U2,T,Types...>{
            constexpr static inline std::size_t index = 2uz;
        };
        template<typename T,typename U1,typename U2,typename U3,typename ...Types>
        struct pack_find<T,U1,U2,U3,Types...>{
            constexpr static inline std::size_t index = pack_find<T,Types...>::index-3uz;
        };
        template<typename T,typename ...Types>
        constexpr inline std::size_t pack_find_i = pack_find<T,Types...>::index;
    }
    template<typename ...Tv>
    class heap_variant{
        void* data;
        std::size_t num;
        void destroy(){
            if(data){
                detail::destroy<Tv...>(std::exchange(data,nullptr),num);
            }
        }
        public:
            template<typename T>
            constexpr static std::size_t index_of = detail::pack_find_i<T,Tv...>;
            constexpr static std::size_t none{std::numeric_limits<std::size_t>::max()};
            heap_variant() : data(nullptr), num(0uz){}
            template<typename T>
            heap_variant(T&& inst) : data(new T(std::forward<T>(inst))), num(index_of<T>){}
            template<typename T,typename ...A>
            heap_variant(emplace_tag_t<T>,A&& ...argv) : data(new T(std::forward<A>(argv)...)), num(index_of<T>){}
            heap_variant(const heap_variant&) = delete;
            heap_variant(heap_variant&& other) noexcept : data(std::exchange(other.data,nullptr)), num(other.num){}
            heap_variant& operator=(const heap_variant&) = delete;
            heap_variant& operator=(heap_variant&& other){
                if(this!=&other){
                    detail::destroy<Tv...>(cppp::shl(data,other.data,nullptr),std::exchange(num,other.num));
                }
                return *this;
            }
            template<typename T,typename ...A>
            void emplace(A&& ...argv){
                destroy();
                data = new T(std::forward<A>(argv)...);
                num = index_of<T>;
            }
            template<std::size_t i,typename ...A>
            void emplace(A&& ...argv){
                destroy();
                using T = compat::index_pack<i,Tv...>;
                data = new T(std::forward<A>(argv)...);
                num = i;
            }
            template<typename T>
            T& get(){
                return *static_cast<T*>(data);
            }
            template<typename T>
            const T& get() const{
                return *static_cast<const T*>(data);
            }
            template<std::size_t i>
            compat::index_pack<i,Tv...>& get(){
                return get<compat::index_pack<i,Tv...>>();
            }
            template<std::size_t i>
            const compat::index_pack<i,Tv...>& get() const{
                return get<compat::index_pack<i,Tv...>>();
            }
            std::size_t index() const{
                return num;
            }
            std::size_t tell() const{
                return empty()?none:num;
            }
            bool empty() const{
                return data == nullptr;
            }
            explicit operator bool(){
                return data;
            }
            ~heap_variant(){
                destroy();
            }
    };
}