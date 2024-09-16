namespace SimpleRTTR
{
    // type is either a fundamental type, a pointer, or is copy assignable
    template<typename VariantType>
    typename std::enable_if<std::is_fundamental_v<VariantType> || std::is_pointer_v<VariantType> || std::is_copy_assignable_v<VariantType>, void>::type
        VariantCopyHelper(const Variant& src, void* dest, const TypeReference& destType)
    {
        VariantType* destPtr = reinterpret_cast<VariantType*>(dest);
        *destPtr = src.GetAs<VariantType>();
        return;
    }

    //specialization for std::string
    template<>
    inline void VariantCopyHelper<std::string>(const Variant& src, void* dest, const TypeReference& destType)
    {
        (void)src;
        (void)dest;
        (void)destType; //unreferenced parameter


        std::string* destPtr = reinterpret_cast<std::string*>(dest);
        Type srcType = src.Type().Type();
        if (srcType == Types().GetType<std::string>())
        {
            *destPtr = src.GetAs<std::string>();
        }
        else if (srcType == Types().GetType<char*>() || srcType == Types().GetType<const char*>())
        {
            *destPtr = src.GetAs<const char*>();
        }
        else
        {
            SIMPLERTTR_ASSERT(!"Attempting to copy string from non-string type");
        }
        return;
    }

    // 
    template<typename VariantType>
    typename std::enable_if<!std::is_fundamental_v<VariantType> && !std::is_pointer_v<VariantType> && !std::is_copy_assignable_v<VariantType>, void>::type
        VariantCopyHelper(const Variant& src, void* dest, const TypeReference& destType)
    {
        if constexpr (std::is_trivially_copyable_v<VariantType> == true)
        {
            std::memcpy(dest, &src, src.Type().Size());
        }
        else
        {
            SIMPLERTTR_ASSERT(!"Not yet implemented");
        }
        return;
    }


    template<typename VariantType> 
    inline void VariantCopy(const Variant& src, void* dest, const TypeReference& destType)
    {
        (void)destType; //unreferenced parameter
        VariantCopyHelper<VariantType>(src, dest, destType);
    }

    template<> inline 
    void VariantCopy<void>(const Variant& src, void* dest, const TypeReference& destType)
    {
        SIMPLERTTR_ASSERT(!"Attempting to copy void type");
        return;
    }

    template<template <typename... > class Tmpl, typename ...Args>
    inline void VariantCopy(const Variant& src, void* dest, const TypeReference& destType)
    {
        VariantCopy<Tmpl<Args...>>(src, dest, destType);
    }


    template<typename VariantType>
    inline std::any PtrToAny(const void* ptr)
    {
        if constexpr(std::is_reference_v<VariantType>) {
            using BaseType = std::remove_reference_t<VariantType>;
			return std::any(*static_cast<BaseType*>(const_cast<void*>(ptr)));
        }
		else if constexpr (std::is_trivially_constructible_v<VariantType> || std::is_copy_constructible_v<VariantType>)
		{
			return std::any(*static_cast<VariantType*>(const_cast<void*>(ptr)));
		}
		else
		{
			SIMPLERTTR_ASSERT(!"Attempting to copy type that isn't trivially constructable or copyable");
			return std::any(0);
		}
    }

    template<> inline std::any PtrToAny<void>(const void* ptr)
    {
        SIMPLERTTR_ASSERT(!"Attempting to copy void type");
        return std::any(0);
    }

    template<template <typename... > class Tmpl, typename ...Args>
    inline std::any PtrToAny(const void* ptr)
    {
        return std::any(*reinterpret_cast<const Tmpl<Args>*>(ptr));
    }
}
