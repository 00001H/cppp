#pragma once
#include<initializer_list>
#include<type_traits>
#include<algorithm>
#include<cstdint>
#include<cmath>
#include<span>
#include<cppp/zeroing-field.hpp>
namespace cppp{
    class bytes{
        zeroing_field<std::byte*> _m;
        zeroing_field<std::size_t> _l;
        zeroing_field<std::size_t> _c;
        void _reallocate(){
            delete[] std::exchange(*_m,*_c?new std::byte[*_c]:nullptr);
        }
        void _appendb(std::uint64_t off,std::byte b){
            (*_m)[*_l+off] = b;
        }
        void _appendm(std::span<const std::byte> m){
            std::ranges::copy(m,*_m+*_l);
            *_l += m.size();
        }
        public:
            bytes() = default;
            bytes(std::initializer_list<std::byte> b){
                append(b);
            }
            void reserve(std::size_t ns){
                do{
                    *_c += std::max(*_c,1uz);
                }while(ns>*_c);
                _reallocate();
            }
            void resize(std::size_t ns,std::byte b=std::byte{0}){
                reserve(ns);
                if(ns>*_l){
                    std::fill(*_m+*_l,*_m+ns,b);
                    *_l = ns;
                }
            }
            void skip(std::size_t amnt){
                *_l += amnt;
                reserve(*_l);
            }
            void append(std::span<const std::byte> b){
                std::size_t _ll = *_l+1uz;
                reserve(_ll);
                _appendm(b);
            }
            void append(std::byte b){
                std::size_t _ll = *_l+1uz;
                reserve(_ll);
                _appendb(0,b);
                _l = _ll;
            }
            void append(std::uint8_t b){
                append(std::byte{b});
            }
            void appendl(std::uint8_t b){
                append(std::byte{b});
            }
            void appendl(std::uint16_t w){
                std::size_t _ll = *_l+2uz;
                reserve(_ll);
                _appendb(0,static_cast<std::byte>(w));
                _appendb(1,static_cast<std::byte>(w>>8uz));
                _l = _ll;
            }
            void appendl(std::uint32_t d){
                std::size_t _ll = *_l+4uz;
                reserve(_ll);
                _appendb(0,static_cast<std::byte>(d));
                _appendb(1,static_cast<std::byte>(d>>8uz));
                _appendb(2,static_cast<std::byte>(d>>16uz));
                _appendb(3,static_cast<std::byte>(d>>24uz));
                _l = _ll;
            }
            void appendl(std::uint64_t q){
                std::size_t _ll = *_l+8uz;
                reserve(_ll);
                _appendb(0,static_cast<std::byte>(q));
                _appendb(1,static_cast<std::byte>(q>>8uz));
                _appendb(2,static_cast<std::byte>(q>>16uz));
                _appendb(3,static_cast<std::byte>(q>>24uz));
                _appendb(4,static_cast<std::byte>(q>>32uz));
                _appendb(5,static_cast<std::byte>(q>>40uz));
                _appendb(6,static_cast<std::byte>(q>>48uz));
                _appendb(7,static_cast<std::byte>(q>>56uz));
                _l = _ll;
            }
            std::byte& operator[](std::size_t ind){
                return (*_m)[ind];
            }
            const std::byte& operator[](std::size_t ind) const{
                return (*_m)[ind];
            }
            std::byte* data(){
                return *_m;
            }
            const std::byte* data() const{
                return *_m;
            }
            using iterator = std::byte*;
            using const_iterator = const std::byte*;
            iterator begin(){
                return *_m;
            }
            const_iterator cbegin() const{
                return *_m;
            }
            const_iterator begin() const{
                return *_m;
            }
            iterator end(){
                return *_m+*_l;
            }
            const_iterator cend() const{
                return *_m+*_l;
            }
            const_iterator end() const{
                return *_m+*_l;
            }
            std::size_t size() const{
                return *_l;
            }
            void fit(){
                if(*_l<*_c){
                    _c = *_l;
                    _reallocate();
                }
            }
            void reallocate(std::size_t sz){
                if(*_c != sz){
                    _c = sz;
                    _reallocate();
                }
            }
            ~bytes(){
                delete[] *_m;
            }
    };
}
