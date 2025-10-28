#pragma once
#include<memory>
#include<utility>
namespace cppp{
    namespace shared_ptr{
        template<typename T,typename ...A>
        std::shared_ptr<T> pnew(A&& ...a){
            return std::shared_ptr<T>(new T(std::forward<A>(a)...));
        }
    }
    namespace unique_ptr{
        template<typename T,typename ...A>
        std::unique_ptr<T> pnew(A&& ...a){
            return std::unique_ptr<T>(new T(std::forward<A>(a)...));
        }
    }
}
