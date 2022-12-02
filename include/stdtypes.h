#pragma once

namespace SimpleRTTR
{
    namespace stdrttr
    {
#if defined(SIMPLERTTR_CUSTOM_ALLOCATOR)
        template<typename ClassType>
        using custom_allocator = SIMPLERTTR_CUSTOM_ALLOCATOR<ClassType>;
#else
        template<typename ClassType>
        using custom_allocator = std::allocator<ClassType>;
#endif

        //The main reason for a separate definition for STL types is to allow for custom allocators which can be used to track
        //  and report allocation usage to better get a handle on how many allocations have been made and optimize it efficiently.
        //  Any data types that have an allocator should be defined here!
        using string = std::basic_string<char, std::char_traits<char>, custom_allocator<char>>;

        template <typename ClassType>
        using vector = std::vector<ClassType, custom_allocator<ClassType>>;
    }
}