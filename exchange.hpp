#pragma once
#include<utility>
namespace cppp{
    namespace detail{
        inline void shl(){}
        template<typename T,typename U>
        void shl(T& x,U&& y){
            x = std::forward<U>(y);
        }
        template<typename T,typename U,typename ...Etc> requires(sizeof...(Etc)>0uz)
        void shl(T& x,U& y,Etc&& ...etc){
            x = std::move(y);
            shl(y,std::forward<Etc>(etc)...);
        }
    }
    template<typename First,typename ...Rest>
    First shl(First& first,Rest&& ...rest){
        First old_value{std::move(first)};
        detail::shl(first,std::forward<Rest>(rest)...);
        return old_value;
    }
}