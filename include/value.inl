namespace SimpleRTTR
{
    Value::Value(stdrttr::string name, class Variant value)
        :
        _Name(name),
        _Value(value)
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

    bool Value::operator== (const Value& other) const
    {
        return _Name == other._Name && _Value == other._Value;
    }

}
