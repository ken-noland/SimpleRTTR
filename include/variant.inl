namespace SimpleRTTR
{
    template<typename VariantType>
    struct CompareInternal
    {
        static inline bool Compare(const std::any& lhs, const std::any& rhs);
    };

    template<typename VariantType>
    bool CompareInternal<VariantType>::Compare(const std::any& lhs, const std::any& rhs)
    {
        VariantType var1 = std::any_cast<VariantType>(lhs);
        VariantType var2 = std::any_cast<VariantType>(rhs);

        if constexpr (
            std::is_integral<VariantType>::value ||
            std::is_pointer_v<VariantType> ||
            std::is_member_function_pointer_v<VariantType>
            )
        {
            return var1 == var2;
        }

        SIMPLERTTR_ASSERT_MSG(false, "Unable to determine equality for non-integral types")
    }

    template<>
    bool CompareInternal<const char*>::Compare(const std::any& lhs, const std::any& rhs)
    {
        const char* var1 = std::any_cast<const char*>(lhs);
        const char* var2 = std::any_cast<const char*>(rhs);

        return var1 == var2 || std::strcmp(var1, var2) == 0;
    }

    template<>
    bool CompareInternal<std::nullptr_t>::Compare(const std::any& lhs, const std::any& rhs)
    {
        return std::type_index(rhs.type()) == std::type_index(typeid(std::nullptr_t));
    }

    template<typename VariantType>
    Variant::Variant(VariantType value)
        :
        _Value(value),
        _Comparator(&CompareInternal<VariantType>::Compare)
    {
    }

    Variant::Variant(const Variant& var)
        :
        _Value(var._Value),
        _Comparator(var._Comparator)
    {
    }

    Variant::Variant(Variant&& var)
        :
        _Value(std::move(var._Value)),
        _Comparator(std::move(var._Comparator))
    {
    }

    Variant& Variant::operator=(const Variant& meta)
    {
        _Value = meta._Value;
        return *this;
    }

    bool Variant::operator==(const Variant& var) const
    {
        if (Type() == var.Type())
        {
            return _Comparator(_Value, var._Value);
        }
        return false;
    }

    bool Variant::operator!=(const Variant& var) const
    {
        return !operator==(var);
    }

    template<typename ObjectType>
    bool Variant::operator==(const ObjectType& var) const
    {
        Variant varType(var);
        return operator==(varType);
    }

    const std::any& Variant::Value() const
    {
        return _Value;
    }


    template<typename VariantType>
    VariantType Variant::GetAs() const
    {
        return std::any_cast<VariantType>(_Value);
    }

    inline const class Type Variant::Type() const
    {
        return Types().GetType(_Value.type());
    }

    inline stdrttr::string Variant::ToString() const
    {
        const class Type& type = Type();
        SIMPLERTTR_ASSERT(type != Type::InvalidType());
        return type.ToString(*this);
    }
}
