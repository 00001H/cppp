#pragma once
#include<cstdint>
#include<set>
namespace cppp{
    template<typename T=std::uint64_t>
    class Freelist{
        T max;
        T size;
        std::set<T,std::greater<T>> freelist;
        public:
            Freelist() : max(0), size(0){}
            T size() const{
                return size;
            }
            T max_size() const{
                return max;
            }
            T allocate(){
                if(freelist.empty()){
                    if(++size > max) max = size;
                    return size - 1;
                }else{
                    T ind = *freelist.begin();
                    freelist.erase(freelist.begin());
                    return ind;
                }
            }
            bool occupied(T ind) const{
                return ind < size && !freelist.contains(ind);
            }
            void deallocate(T ind){
                freelist.try_emplace(ind);
                T ls;
                while(!freelist.empty()&&(*freelist.begin())==(ls = size-1)){
                    freelist.erase(freelist.begin());
                    size = ls;
                }
            }
    };
}
