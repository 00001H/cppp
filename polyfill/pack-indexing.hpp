#pragma once
#include<cstdint>
namespace cppp::compat{
    #ifdef __cpp_pack_indexing
    template<std::size_t i,typename ...Types>
    using index_pack = Types...[i];
    #else
    namespace detail{
        template<std::size_t i,typename ...Types>
        struct do_index{};
        template<typename T,typename ...Etc>
        struct do_index<0uz,T,Etc...>{
            using type = T;
        };
        template<typename U1,typename T,typename ...Etc>
        struct do_index<1uz,U1,T,Etc...>{
            using type = T;
        };
        template<typename U1,typename U2,typename T,typename ...Etc>
        struct do_index<2uz,U1,U2,T,Etc...>{
            using type = T;
        };
        template<typename U1,typename U2,typename U3,typename T,typename ...Etc>
        struct do_index<3uz,U1,U2,U3,T,Etc...>{
            using type = T;
        };
        template<std::size_t ind,typename U1,typename U2,typename U3,typename U4,typename ...Etc>
        struct do_index<ind,U1,U2,U3,U4,Etc...>{
            using type = do_index<ind-4uz,Etc...>::type;
        };
    }
    template<std::size_t i,typename ...Types> requires(i < sizeof...(Types))
    using index_pack = detail::do_index<i,Types...>::type;
    #endif
}