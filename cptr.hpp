#pragma once
#include<concepts>
#include"ptr.hpp"
namespace cppp{
    template<typename T>
    class scptr : public ptr<T>{
        public:
            using ptr<T>::ptr;
            scptr(T*&& p) : ptr<T>(p){}
            scptr(const scptr& cpy) : ptr<T>(cpy?new T(*cpy):nullptr){}
            scptr& operator=(const scptr& cpy){
                this->reset(cpy?new T(*cpy):nullptr);
                return *this;
            }
    };
    template<typename T>
    class dcptr{
        template<typename U>
        friend class dcptr;
        ptr<T> p;
        void*(*cloner)(const void*);
        template<std::derived_from<T> U>
        dcptr(U* p) : p(p), cloner([](const void* p) -> void* {return new U(*static_cast<const U*>(p));}){}
        T* _clone() const{
            if(const T* ptr=p.get()){
                return static_cast<T*>(cloner(ptr));
            }
            return nullptr;
        }
        public:
            dcptr() : p(nullptr), cloner(nullptr){}
            dcptr(const dcptr& other) : p(other._clone()), cloner(other.cloner){}
            dcptr(dcptr&& other) noexcept : p(std::move(other.p)), cloner(other.cloner){}
            template<std::derived_from<T> U>
            dcptr(const dcptr<U>& other) : p(other._clone()), cloner(other.cloner){}
            template<std::derived_from<T> U>
            dcptr(dcptr<U>&& other) noexcept : p(std::move(other.p)), cloner(other.cloner){}
            template<typename ...Ca>
            static dcptr<T> construct(Ca&& ...ca){
                return {new T(std::forward<Ca>(ca)...)};
            }
            T* get(){
                return p.get();
            }
            const T* get() const{
                return p.get();
            }
            T& operator*(){
                return *p;
            }
            const T& operator*() const{
                return *p;
            }
            T* operator->(){
                return p.get();
            }
            const T* operator->() const{
                return p.get();
            }
            dcptr& operator=(const dcptr& other){
                p.reset(other._clone());
                cloner = other.cloner;
                return *this;
            }
            dcptr& operator=(dcptr&& other){
                p = std::move(other.p);
                cloner = other.cloner;
                return *this;
            }
            template<std::derived_from<T> U>
            dcptr& operator=(const dcptr<U>& other){
                p.reset(other._clone());
                cloner = other.cloner;
                return *this;
            }
            template<std::derived_from<T> U>
            dcptr& operator=(dcptr<U>&& other){
                p = std::move(other);
                cloner = other.cloner;
                return *this;
            }
            //Destruction handled by ptr, no need for custom dtor.
    };
    template<typename T>
    using cptr = std::conditional_t<std::is_polymorphic_v<T>,dcptr<T>,scptr<T>>;
}
