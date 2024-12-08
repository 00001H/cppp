#pragma once
namespace cppp{
    template<template<typename...> typename T,typename ...A>
    struct template_bind{
        template<typename ...T2>
        struct wrapper{
            using type = T<A...,T2...>;
        };
        template<typename ...T2>
        using type = T<A...,T2...>;
    };
}