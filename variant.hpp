#include<type_traits>
#include<exception>
#include<utility>
namespace cppp{
    template<auto enm_val,typename T>
    struct var_type{
        constexpr static decltype(enm_val) enumeration{enm_val};
        using type = T;
    };
    namespace detail{
        template<auto enm_val>
        struct construct_enum_t{
            constexpr static decltype(enm_val) enumeration{enm_val};
        };
        template<auto enm_val,typename ...etc>
        struct _query{
            using type = void;
        };
        template<auto enm_val,typename U,typename ...etc>
        struct _query<enm_val,U,etc...>{
            using type = std::conditional_t<
                enm_val == U::enumeration,
                typename U::type,
                typename _query<enm_val,etc...>::type>;
        };
        template<auto enm_val,typename ...etc>
        using _query_t = _query<enm_val,etc...>::type;
        template<typename enm>
        void _delete(void*,enm){}
        template<typename enm,typename first,typename ...types>
        void _delete(void* data,enm enumeration){
            if(enumeration == first::enumeration){
                delete static_cast<first::type*>(data);
            }else{
                _delete<enm,types...>(data,enumeration);
            }
        }
        template<typename enm>
        bool _is_small(enm){
            throw bad_variant_alternative();
        }
        template<typename enm,typename first,typename ...types>
        bool _is_small(enm enumeration){
            if(enumeration == first::enumeration){
                return small<first::type>;
            }else{
                return _is_small<enm,types...>(enumeration);
            }
        }
        template<typename enm>
        void _destroy(void*,enm){}
        template<typename enm,typename first,typename ...types>
        void _destroy(void* data,enm enumeration){
            if(enumeration == first::enumeration){
                using ft = first::type;
                static_cast<ft*>(data)->~ft();
            }else{
                _destroy<enm,types...>(data,enumeration);
            }
        }
        struct bad_variant_alternative : std::exception{
            using std::exception::exception;
        };
        template<typename enm,typename ...types>
        struct _constructor{
            template<typename ...A>
            void* _construct(enm,A&&...){
                throw bad_variant_alternative();
            }
        };
        template<typename enm,typename F,typename ...types>
        struct _constructor<enm,F,types...>{
            template<typename ...A>
            void _construct_ip(void* to,enm enumeration,A&& ...args){
                if(enumeration == F::enumeration){
                    using vt = F::type;
                    new(to) vt(std::forward<A>(args)...);
                }
                _constructor<enm,types...>::_construct_ip(to,enumeration,std::forward<A>(args)...);
            }
            template<typename ...A>
            void* _construct(enm enumeration,A&& ...args){
                if(enumeration == F::enumeration){
                    using vt = F::type;
                    return new vt(std::forward<A>(args)...);
                }
                return _constructor<enm,types...>::_construct(enumeration,std::forward<A>(args)...);
            }
        };
        constexpr static inline std::size_t SMALL_BUFFER_SIZE = sizeof(void*);
        constexpr static inline std::size_t SMALL_BUFFER_ALIGNMENT = alignof(void*);
        template<typename T>
        concept small = (sizeof(T)<=SMALL_BUFFER_SIZE)&&(alignof(T)<=SMALL_BUFFER_ALIGNMENT);
    }
    template<auto enm_val>
    constexpr inline detail::construct_enum_t<enm_val> construct_enum{};
    template<typename enm,enm null_v,typename ...types> requires(std::is_enum_v<enm>)
    class variant{
        union{
            void* data;
            std::byte small_buffer[SMALL_BUFFER_SIZE];
        } data;
        enm held;
        template<enm enm_val,typename ...A>
        void* _construct(A&& ...a){
            using vt = detail::_query_t<enm_val,types...>;
            return new vt(std::forward<A>(a)...);
        }
        template<enm enm_val,typename ...A>
        void _construct_ip(A&& ...a){
            using vt = detail::_query_t<enm_val,types...>;
            new(data.small_buffer) vt(std::forward<A>(a)...);
        }
        void _destroy(){
            if(held != null_v){
                if(detail::_is_small<enm,types...>(held)){
                    detail::_destroy<enm,types...>(data.small_buffer,held);
                }else{
                    detail::_delete<enm,types...>(data.data,held);
                }
                held = null_v;
            }
        }
        public:
            template<enm enm_val,typename ...A> requires(!detail::small<detail::_query_t<enm_val,types...>>)
            variant(detail::construct_enum_t<enm_val>,A&& ...args) : data{.data=_construct<enm_val>(std::forward<A>(args)...)}, held(enm_val){}
            template<enm enm_val,typename ...A> requires(detail::small<detail::_query_t<enm_val,types...>>)
            variant(detail::construct_enum_t<enm_val>,A&& ...args) : data{.small_buffer{}}, held(enm_val){
                _construct_ip<enm_val>(std::forward<A>(args)...);
            }
            variant(const variant&) = delete;
            variant(variant&& other) : data(other.data), held(std::exchange(other.held,null_v)){}
            variant& operator=(const variant&) = delete;
            variant& operator=(variant&& other){
                if(this!=&other){
                    _destroy();
                    data = other.data;
                    held = std::exchange(other.held,null_v);
                }
                return *this;
            }
            template<enm enm_val,typename ...A>
            void emplace(A&& ...args){
                _destroy();
                held = enm_val;
                if constexpr(detail::small<enm_val>){
                    _construct_ip<enm_val>(std::forward<A>(args)...);
                }else{
                    data.data = _construct<enm_val>(std::forward<A>(args)...);
                }
            }
            template<typename ...A>
            void set(enm enm_val,A&& ...args){
                _destroy();
                held = enm_val;
                if(detail::_is_small<enm,types...>(enm_val)){
                    detail::_constructor<enm,types...>::_construct_ip(data.small_buffer,enm_val,std::forward<A>(args)...);
                }else{
                    data.data = detail::_constructor<enm,types...>::_construct(enm_val,std::forward<A>(args)...);
                }
            }
            template<enm enm_val>
            detail::_query_t<enm_val,types...>& get(){
                using vt = detail::_query_t<enm_val,types...>;
                if constexpr(detail::small<vt>){
                    return *static_cast<vt*>(data.small_buffer);
                }else{
                    return *static_cast<vt*>(data.data);
                }
            }
            template<enm enm_val>
            const detail::_query_t<enm_val,types...>& get() const{
                using vt = detail::_query_t<enm_val,types...>;
                if constexpr(detail::small<vt>){
                    return *static_cast<const vt*>(data.small_buffer);
                }else{
                    return *static_cast<const vt*>(data.data);
                }
            }
            enm current() const{
                return held;
            }
            void hard_replace(enm other){
                held = other;
            }
            ~variant(){
                _destroy();
            }
    };
}
/*

#include<iostream>
#include<cppp/variant.hpp>
enum Basic{
    i,f,ff
};
struct Foo{
    Foo(){
        std::cout << "Ctor\n";
    }
    ~Foo(){
        std::cout << "Dtor\n";
    }
};
int main(){
    cppp::variant<Basic,
        cppp::var_type<i,int>,
        cppp::var_type<f,float>,
        cppp::var_type<ff,Foo>
    > vt{
        cppp::construct_enum<ff>
    };
    std::cout << (vt.current() == ff) << std::endl;
    vt.set<i>(3);
    std::cout << (vt.current() == i) << (vt.current() == f) << std::endl;
    std::cout << vt.get<i>() << std::endl;
    return 0;
}
*/