#pragma once

namespace SimpleRTTR
{
    //--
    //String manipulation helpers
    inline std::string& trimr(std::string& str)
    {
        if (!str.size()) return str;
        // find the last whitespace chr, then delete from there to end.
        std::string::iterator iter = --str.end();
        while (isspace(*iter)) --iter;
        str.erase(++iter, str.end());
        return str;
    }

    inline std::string& triml(std::string& str)
    {
        if (!str.size()) return str;
        // find first non-whitespace, then delete from begin to there.
        std::string::iterator iter = str.begin();
        while (iter != str.end() && isspace(*iter)) ++iter;
        str.erase(str.begin(), iter);
        return str;
    }

    inline std::string& trim(std::string& str)
    {
        return trimr(triml(str));
    }

    // Primary template for detecting the equality operator
    template <typename, typename = std::void_t<>>
    struct has_equal_operator : std::false_type {};

    // Specialization that will be enabled if the type has `==` operator
    template <typename T>
    struct has_equal_operator<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>> : std::true_type {};

    // Convenience variable template (C++17)
    template <typename T>
    inline constexpr bool has_equal_operator_v = has_equal_operator<T>::value;


    // Helper to check if a type is iterable
    template <typename T>
    class IsIterableHelper {
    private:
        template <typename U>
        static auto test(int) -> decltype(std::begin(std::declval<U&>()) != std::end(std::declval<U&>()), std::true_type{});

        template <typename U>
        static std::false_type test(...);

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };
}
