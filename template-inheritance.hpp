#pragma once
#include<concepts>
namespace cppp{
    template<typename T,template<typename...> typename Te>
    struct derived_from_instantiation{
        constexpr static bool value = false;
    };
    template<template<typename...> typename Te,typename ...Values,std::derived_from<Te<Values...>> cls>
    struct derived_from_instantiation<cls,Te>{
        constexpr static bool value = true;
    };
    template<typename T,template<typename...> typename Te>
    using derived_from_instantiation_v = derived_from_instantiation::value;
}
