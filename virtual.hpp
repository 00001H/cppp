#pragma once
namespace cppp{
    class virtual_class{
        protected:
            constexpr virtual_class(const virtual_class&) noexcept = default;
            constexpr virtual_class(virtual_class&&) noexcept = default;
            constexpr virtual_class& operator=(const virtual_class&) noexcept = default;
            constexpr virtual_class& operator=(virtual_class&&) noexcept = default;
        public:
            constexpr virtual_class() noexcept = default;
            constexpr virtual ~virtual_class() noexcept = default;
    };
}
