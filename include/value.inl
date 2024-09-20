namespace SimpleRTTR
{
    Value::Value(std::string name, class Variant value)
        :
        _Name(name),
        _Value(value),
        _Meta()
    {

    }

    const std::string& Value::name() const
    {
        return _Name;
    }

    const class Variant& Value::value() const
    {
        return _Value;
    }

    std::size_t Value::hash() const
    {
        std::size_t seed = 0;
        hash_combine(seed, _Name, _Value, _Meta);
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
