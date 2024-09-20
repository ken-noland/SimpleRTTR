namespace SimpleRTTR
{
    template<typename VariantType> inline std::string VariantToString(const Variant& var)
    {
        SIMPLERTTR_ASSERT(!"Not yet implemented");
        return std::string();
    }

    template<> inline std::string VariantToString<char>(const Variant& var)
    {
        return std::string(1, var.get_as<char>());
    }

    template<> inline std::string VariantToString<short>(const Variant& var)
    {
        return std::string(std::to_string(var.get_as<short>()));
    }

    template<> inline std::string VariantToString<int>(const Variant& var)
    {
        return std::string(std::to_string(var.get_as<int>()));
    }

    template<> inline std::string VariantToString<const char*>(const Variant& var)
    {
        return std::string(var.get_as<const char*>());
    }

    template<template <typename... > class Tmpl, typename ...Args>
    class SimpleContainerVariantToStringHelper { 
    public:
        inline std::string to_string(const Variant& var) const
        {
            using container_type = Tmpl<Args...>;
            using iterator_type = typename container_type::const_iterator;
            using value_type = typename container_type::value_type;

            std::stringstream stringStream;

            stringStream << "{ ";

            container_type vec = var.get_as<container_type>();
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
        inline std::string to_string(const Variant& var) const
        {
            SIMPLERTTR_ASSERT(!"Not yet implemented");
            return std::string();
        }
    };

    template<typename ...Args>
    class ContainerVariantToStringHelper<std::vector, Args...> {
    public:
        inline std::string to_string(const Variant& var) const
        {
            SimpleContainerVariantToStringHelper<std::vector, Args...> helper;
            return helper.to_string(var);
        }
    };


    template<typename ...Args>
    class ContainerVariantToStringHelper<std::initializer_list, Args...> {
    public:
        inline std::string to_string(const Variant& var) const
        {
            SimpleContainerVariantToStringHelper<std::initializer_list, Args...> helper;
            return helper.to_string(var);
        }
    };

    template<template <typename... > class Tmpl, typename ...Args> 
    inline std::string VariantToString(const Variant& var)
    {
        ContainerVariantToStringHelper<Tmpl, Args...> helper;
        return helper.to_string(var);
    }
}

