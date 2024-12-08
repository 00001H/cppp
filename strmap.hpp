#pragma once
#include<unordered_map>
#include<string_view>
#include<string>
namespace cppp{
    namespace detail{
        struct str_hash{
            constexpr std::size_t operator()(const std::u8string& v) const noexcept{
                return std::hash<std::u8string>()(v);
            }
            constexpr std::size_t operator()(const std::u8string_view& v) const noexcept{
                return std::hash<std::u8string_view>()(v);
            }
        };
        struct str_equ{
            constexpr bool operator()(const std::u8string& lhs,const std::u8string& rhs) const noexcept{
                return lhs == rhs;
            }
            constexpr bool operator()(const std::u8string_view& lhs,const std::u8string& rhs) const noexcept{
                return lhs == rhs;
            }
            constexpr bool operator()(const std::u8string& lhs,const std::u8string_view& rhs) const noexcept{
                return lhs == rhs;
            }
            constexpr bool operator()(const std::u8string_view& lhs,const std::u8string_view& rhs) const noexcept{
                return lhs == rhs;
            }
        };
    }
    template<typename E>
    using strmap = std::unordered_map<std::u8string,E,detail::str_hash,detail::str_equ>;
}
