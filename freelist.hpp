#pragma once
#include<cstdint>
#include<queue>
namespace cppp{
    template<typename T=std::uint64_t>
    class Freelist{
        T max;
        T length;
        std::priority_queue<T> freelist;
        public:
            Freelist() : max(0), length(0){}
            T size() const{
                return length;
            }
            T max_size() const{
                return max;
            }
            T allocate(){
                if(freelist.empty()){
                    if(++length > max) max = length;
                    return length - 1;
                }else{
                    std::size_t ind = freelist.top();
                    freelist.pop();
                    return ind;
                }
            }
            void deallocate(T ind){
                freelist.push(ind);
                T ls;
                while(!freelist.empty()&&freelist.top()==(ls = length-1)){
                    freelist.pop();
                    length = ls;
                }
            }
    };
}
