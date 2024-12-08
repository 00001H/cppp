#pragma once
#include<ranges>
namespace cppp{
    namespace detail{
        struct keys_tag{};
        struct values_tag{};
        
        struct first{
            template<typename K,typename V>
            V& operator()(std::pair<K,V>& p){
                return p.first;
            }
            template<typename K,typename V>
            const V& operator()(const std::pair<K,V>& p){
                return p.first;
            }
        };
        struct second{
            template<typename K,typename V>
            V& operator()(std::pair<K,V>& p){
                return p.second;
            }
            template<typename K,typename V>
            const V& operator()(const std::pair<K,V>& p){
                return p.second;
            }
        };
        constexpr inline auto first_view = std::ranges::views::transform(detail::first{});
        constexpr inline auto second_view = std::ranges::views::transform(detail::second{});
        template<std::ranges::range R>
        decltype(auto) operator|(R&& lhs,keys_tag){
            return std::forward<R>(lhs) | first_view;
        }
        template<std::ranges::range R>
        decltype(auto) operator|(R&& lhs,values_tag){
            return std::forward<R>(lhs) | second_view;
        }
    }
    constexpr inline detail::keys_tag map_keys;
    constexpr inline detail::values_tag map_values;
}