namespace SimpleRTTR
{
    PropertyData::PropertyData(stdrttr::string name, const class Type& type, std::size_t offset, PropertyFlagsBits flags)
        :
        _Name(name),
        _Type(type),
        _Offset(offset),
        _Flags(flags)
    {
    }

    PropertyData::PropertyData(PropertyData&& data)
        :
        _Name(std::move(data._Name)),
        _Type(data._Type),
        _Offset(data._Offset),
        _Flags(data._Flags)
    {
    }

    PropertyData::PropertyData(const PropertyData& data)
        :
        _Name(data._Name),
        _Type(data._Type),
        _Offset(data._Offset),
        _Flags(data._Flags)
    {

    }

    const stdrttr::string& PropertyData::Name() const
    {
        return _Name;
    }

    const Type& PropertyData::Type() const
    {
        return _Type;
    }

    const std::size_t PropertyData::Offset() const
    {
        return _Offset;
    }


    const PropertyData::PropertyFlagsBits& PropertyData::Flags() const
    {
        return _Flags;
    }

    Property::Property(Property&& prop)
        :
        _PropData(prop._PropData)
    {

    }

    Property::Property(const Property& prop)
        :
        _PropData(prop._PropData)
    {

    }


    Property::Property(const PropertyData& propData)
        :
        _PropData(propData)
    {

    }

    Property& Property::operator=(const Property& prop)
    {
        std::memcpy(&_PropData, &prop._PropData, sizeof(PropertyData));
        return *this;
    }

    const stdrttr::string& Property::Name() const
    {
        return _PropData.Name();
    }

    const std::size_t Property::Offset() const
    {
        return _PropData.Offset();
    }

    const class Type& Property::Type() const
    {
        return _PropData.Type();
    }

    bool Property::IsConst()
    {
        //TODO
        return false;
    }

    bool Property::IsPointer()
    {
        //TODO
        return false;
    }

}