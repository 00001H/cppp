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
            void* _construct(enm enumeration,A&& ...args){
                if(enumeration == F::enumeration){
                    using vt = F::type;
                    return new vt(std::forward<A>(args)...);
                }
                _constructor<enm,types...>::_construct(enumeration,std::forward<A>(args)...);
            }
        };
    }
    template<auto enm_val>
    constexpr inline detail::construct_enum_t<enm_val> construct_enum{};
    template<typename enm,typename ...types> requires(std::is_enum_v<enm>)
    class variant{
        void* data;
        enm held;
        template<enm enm_val,typename ...A>
        void* _construct(A&& ...a){
            using vt = detail::_query<enm_val,types...>::type;
            return new vt(std::forward<A>(a)...);
        }
        void _destroy(){
            if(data){
                detail::_delete<enm,types...>(std::exchange(data,nullptr),held);
            }
        }
        public:
            template<enm enm_val,typename ...A>
            variant(detail::construct_enum_t<enm_val>,A&& ...args) : data(_construct<enm_val>(std::forward<A>(args)...)), held(enm_val){}
            template<enm enm_val,typename ...A>
            void set(A&& ...args){
                _destroy();
                held = enm_val;
                data = _construct<enm_val>(std::forward<A>(args)...);
            }
            template<enm enm_val>
            detail::_query<enm_val,types...>::type& get(){
                return *static_cast<detail::_query<enm_val,types...>::type*>(data);
            }
            template<enm enm_val>
            const detail::_query<enm_val,types...>::type& get() const{
                return *static_cast<const detail::_query<enm_val,types...>::type*>(data);
            }
            enm current() const{
                return held;
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