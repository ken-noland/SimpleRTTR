#pragma once

namespace SimpleRTTR
{
    class Variant
    {
    public:
        using StorageType = std::aligned_storage_t<64, alignof(std::max_align_t)>;

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

        // copy the value to the given pointer
        inline void copy_to(void* dest, const Type& destType) const;

        inline const SimpleRTTR::Type& type() const;

        inline std::size_t hash() const;

        inline std::string to_string() const;

    protected:
        StorageType _storage;
        const SimpleRTTR::Type& _type;

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