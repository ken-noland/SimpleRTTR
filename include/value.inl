namespace SimpleRTTR
{
    Value::Value(stdrttr::string name, class Variant value)
        :
        _Name(name),
        _Value(value),
        _Meta()
    {

    }

    const stdrttr::string& Value::Name() const
    {
        return _Name;
    }

    const class Variant& Value::Variant() const
    {
        return _Value;
    }

    std::size_t Value::Hash() const
    {
        std::size_t seed = 0;
        HashCombine(seed, _Name, _Value, _Meta);
        return seed;
    }


    bool Value::operator== (const Value& other) const
    {
        return _Name == other._Name && _Value == other._Value;
    }

    inline MetaContainer& _InternalGetMetadata(class Value& value)
    {
        return value._Meta;
    }
}
