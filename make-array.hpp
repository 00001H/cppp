#pragma once
#include<type_traits>
#include<utility>
#include<array>
namespace cppp{
    template<typename T,typename ...Args>
    std::array<T,sizeof...(Args)> make_array(Args&& ...args){
        return {std::forward<Args>(args)...};
    }
}