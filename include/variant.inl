namespace SimpleRTTR
{
    template<typename VariantType>
    Variant::Variant(VariantType value)
        :
        _Value(value)
    {
    }

    Variant::Variant(const Variant& var)
        :
        _Value(var._Value)
    {
    }

    Variant::Variant(Variant&& var)
        :
        _Value(std::move(var._Value))
    {
    }

    Variant& Variant::operator=(const Variant& meta)
    {
        _Value = meta._Value;
        return *this;
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

    inline const class Type& Variant::Type() const
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
