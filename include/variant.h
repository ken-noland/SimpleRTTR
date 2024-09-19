#pragma once

namespace SimpleRTTR
{
    class Variant
    {
    public:
        using StorageType = std::aligned_storage_t<64, alignof(std::max_align_t)>;

        template<typename VariantType>
        inline Variant(VariantType value);
        inline Variant(void* value, Type type);

        inline Variant(const Variant& var);
        inline Variant(Variant&& var);

        inline ~Variant();

        inline Variant& operator=(const Variant& meta);

        inline bool operator==(const Variant& var) const;
        inline bool operator!=(const Variant& var) const;

        template<typename ObjectType>
        inline bool operator==(const ObjectType& var) const;

        template<typename VariantType>
        inline VariantType GetAs() const;

        // copy the value to the given pointer
        inline void CopyTo(void* dest) const;

        inline const class TypeReference Type() const;

        inline std::size_t Hash() const;

        inline stdrttr::string ToString() const;

    protected:
        StorageType _storage;

        using CopyFunc = void (*)(StorageType&, const StorageType&);
        CopyFunc _copyFunc;

        using DeleteFunc = void (*)(StorageType&);
        DeleteFunc _deleteFunc;

        using EqualityFunc = bool (*)(const StorageType&, const StorageType&);
        EqualityFunc _equalityFunc;

        std::type_index _typeIndex;

        inline void Destroy();
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
    inline stdrttr::string VariantToString(const Variant& var);

    template<template <typename... > class Tmpl, typename ...Args>
    inline stdrttr::string VariantToString(const Variant& var);
}