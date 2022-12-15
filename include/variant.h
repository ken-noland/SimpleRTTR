#pragma once

namespace SimpleRTTR
{
    class Variant
    {
    public:
        template<typename VariantType>
        inline Variant(VariantType value);
        inline Variant(const Variant& var);
        inline Variant(Variant&& var);
        inline Variant& operator=(const Variant& meta);

        inline const std::any& Value() const;

        template<typename VariantType>
        inline VariantType GetAs() const;

        inline const class Type& Type() const;

        inline stdrttr::string ToString() const;

    protected:
        std::any _Value;
    };

    template<typename VariantType>
    inline stdrttr::string VariantToString(const Variant& var);

    template<template <typename... > class Tmpl, typename ...Args>
    inline stdrttr::string VariantToString(const Variant& var);
}