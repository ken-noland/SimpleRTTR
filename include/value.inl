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
}
