namespace SimpleRTTR
{
    template<typename VariantType> inline stdrttr::string VariantToString<>(const Variant& var)
    {
        SIMPLERTTR_ASSERT(!"Not yet implemented");
        return stdrttr::string();
    }

    template<> inline stdrttr::string VariantToString<char>(const Variant& var)
    {
        return stdrttr::string(1, var.GetAs<char>());
    }

    template<> inline stdrttr::string VariantToString<short>(const Variant& var)
    {
        return stdrttr::string(std::to_string(var.GetAs<short>()));
    }

    template<> inline stdrttr::string VariantToString<int>(const Variant& var)
    {
        return stdrttr::string(std::to_string(var.GetAs<int>()));
    }

    template<> inline stdrttr::string VariantToString<const char*>(const Variant& var)
    {
        return stdrttr::string(var.GetAs<const char*>());
    }

    template<template <typename... > class Tmpl, typename ...Args>
    class SimpleContainerVariantToStringHelper { 
    public:
        inline stdrttr::string ToString(const Variant& var) const
        {
            using container_type = Tmpl<Args...>;
            using iterator_type = typename container_type::const_iterator;
            using value_type = typename container_type::value_type;

            stdrttr::stringstream stringStream;

            stringStream << "{ ";

            container_type vec = var.GetAs<container_type>();
            iterator_type iter = vec.begin();
            while (true)
            {
                stringStream << VariantToString<value_type>(*iter);
                ++iter;
                if (iter != vec.end())
                {
                    stringStream << ", ";
                }
                else
                {
                    break;
                }
            }

            stringStream << " }";

            return stringStream.str();
        }
    };

    template<template <typename... > class Tmpl, typename ...Args>
    class ContainerVariantToStringHelper {
    public:
        inline stdrttr::string ToString(const Variant& var) const
        {
            SIMPLERTTR_ASSERT(!"Not yet implemented");
            return stdrttr::string();
        }
    };

    template<typename ...Args>
    class ContainerVariantToStringHelper<std::vector, Args...> {
    public:
        inline stdrttr::string ToString(const Variant& var) const
        {
            SimpleContainerVariantToStringHelper<std::vector, Args...> helper;
            return helper.ToString(var);
        }
    };


    template<typename ...Args>
    class ContainerVariantToStringHelper<std::initializer_list, Args...> {
    public:
        inline stdrttr::string ToString(const Variant& var) const
        {
            SimpleContainerVariantToStringHelper<std::initializer_list, Args...> helper;
            return helper.ToString(var);
        }
    };

    template<template <typename... > class Tmpl, typename ...Args> 
    inline stdrttr::string VariantToString(const Variant& var)
    {
        ContainerVariantToStringHelper<Tmpl, Args...> helper;
        return helper.ToString(var);
    }
}

