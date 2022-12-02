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

        template<typename VariantType>
        inline VariantType GetAs() const;

        inline const class Type& Type() const;

    protected:
        std::any _Value;
    };
}