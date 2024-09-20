#pragma once

namespace SimpleRTTR
{
    class Variant
    {
    public:
        template<typename VariantType>
        inline Variant(VariantType value);
        inline Variant(void* value, const SimpleRTTR::Type& type);

        inline Variant(const Variant& var);
        inline Variant(Variant&& var);

        inline ~Variant();

        inline Variant& operator=(const Variant& meta);

        inline bool operator==(const Variant& var) const;
        inline bool operator!=(const Variant& var) const;

        template<typename ObjectType>
        inline bool operator==(const ObjectType& var) const;

        template<typename VariantType>
        inline VariantType get_as() const;

        inline SimpleRTTR::Type type() const;

        inline std::size_t hash() const;
        inline std::string to_string() const;

        inline const void* ptr() const;

    protected:
        std::byte _storage[64];
        TypeReference _typeRef;

        // we need to store the functions here for both speed and 
        // stability reasons(when shutting down, we might be trying 
        // to destroy variants that have had their types already 
        // destroyed)

        TypeFunctions::CopyConstructorFunction  _copyFunc;
        TypeFunctions::MoveConstructorFunction  _moveFunc;
        TypeFunctions::DestructorFunction       _destroyFunc;

        TypeFunctions::EqualOperatorFunction    _equalFunc;

        inline void destroy();
    };


    template<typename VariantType>
    inline void VariantCopy(const Variant& src, void* dest, const TypeReference& destType);

    template<template <typename... > class Tmpl, typename ...Args>
    inline void VariantCopy(const Variant& src, void* dest, const TypeReference& destType);

    template<typename VariantType>
    inline std::any PtrToAny(const void*);

    template<template <typename... > class Tmpl, typename ...Args>
    inline std::any PtrToAny(const void*);

    template<typename VariantType>
    inline std::string VariantToString(const Variant& var);

    template<template <typename... > class Tmpl, typename ...Args>
    inline std::string VariantToString(const Variant& var);
}