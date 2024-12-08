#pragma once
#include<utility>
namespace cppp{
    template<typename T>
    struct vec2{
        T x;
        T y;
        vec2() : x(0), y(0){}
        vec2(T x,T y) : x(x), y(y){}
        template<std::convertible_to<T> U>
        vec2(vec2<U> conv) : x(conv.x), y(conv.y){}
        vec2& operator+=(vec2 other){
            x += other.x;
            y += other.y;
            return *this;
        }
        vec2 operator+(this vec2 lhs,vec2 rhs){
            return lhs += rhs;
        }
        vec2& operator-=(vec2 other){
            x -= other.x;
            y -= other.y;
            return *this;
        }
        vec2 operator-(this vec2 lhs,vec2 rhs){
            return lhs -= rhs;
        }
        vec2& operator*=(vec2 other){
            x *= other.x;
            y *= other.y;
            return *this;
        }
        vec2 operator*(this vec2 lhs,vec2 rhs){
            return lhs *= rhs;
        }
        vec2& operator/=(vec2 other){
            x /= other.x;
            y /= other.y;
            return *this;
        }
        vec2 operator/(this vec2 lhs,vec2 rhs){
            return lhs /= rhs;
        }
        vec2& operator+=(T scalar){
            x += scalar;
            y += scalar;
            return *this;
        }
        vec2 operator+(this vec2 lhs,T scalar){
            return lhs += scalar;
        }
        vec2& operator-=(T scalar){
            x -= scalar;
            y -= scalar;
            return *this;
        }
        vec2 operator-(this vec2 lhs,T scalar){
            return lhs -= scalar;
        }
        vec2& operator*=(T scalar){
            x *= scalar;
            y *= scalar;
            return *this;
        }
        vec2 operator*(this vec2 lhs,T scalar){
            return lhs *= scalar;
        }
        vec2& operator/=(T scalar){
            x /= scalar;
            y /= scalar;
            return *this;
        }
        vec2 operator/(this vec2 lhs,T scalar){
            return lhs /= scalar;
        }
        bool operator==(vec2 other) const{
            return x==other.x&&y==other.y;
        }
    };
}
namespace std{
    template<typename T>
    struct hash<cppp::vec2<T>>{
        constexpr size_t operator()(cppp::vec2<T> v) const noexcept{
            hash<T> h;
            return h(v.x)^(h(v.y)<<2);
        }
    };
}
