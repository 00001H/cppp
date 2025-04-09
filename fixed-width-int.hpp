#pragma once
#include<concepts>
#include<cstdint>
namespace cppp{
    template<typename T>
    concept unsigend_fixed_width_true_int = std::same_as<T,std::uint16_t>
                                         || std::same_as<T,std::uint32_t>
                                         || std::same_as<T,std::uint64_t>;
    template<typename T>
    concept unsigend_fixed_width = std::same_as<T,std::uint8_t>
                                || unsigend_fixed_width_true_int<T>;
    template<typename T>
    concept sigend_fixed_width_true_int = std::same_as<T,std::int16_t>
                                          || std::same_as<T,std::int32_t>
                                          || std::same_as<T,std::int64_t>;
    template<typename T>
    concept sigend_fixed_width = std::same_as<T,std::int8_t>
                              || sigend_fixed_width_true_int<T>;
}
