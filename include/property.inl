namespace SimpleRTTR
{
    inline PropertyData& _InternalGetPropertyDataRef(Property& prop);

    PropertyData::PropertyData(stdrttr::string name, const class Type& type, std::size_t offset)
        :
        Name(name),
        Type(type),
        Offset(offset)
    {
    }

    PropertyData::PropertyData(PropertyData&& data)
        :
        Name(std::move(data.Name)),
        Type(data.Type),
        Offset(data.Offset),
        Flags(data.Flags),
        Meta(std::move(data.Meta))
    {
    }

    PropertyData::PropertyData(const PropertyData& data)
        :
        Name(data.Name),
        Type(data.Type),
        Offset(data.Offset),
        Flags(data.Flags),
        Meta(data.Meta)
    {

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
        return _PropData.Name;
    }

    const std::size_t Property::Offset() const
    {
        return _PropData.Offset;
    }

    const class Type& Property::Type() const
    {
        return _PropData.Type;
    }

    const Property::MetaList& Property::Meta() const
    {
        return _PropData.Meta;
    }

    void Property::ForEach(Property::MetaFunction eval) const
    {
        std::for_each(Meta().begin(), Meta().end(), eval);
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

    inline PropertyData& _InternalGetPropertyDataRef(Property& prop)
    {
        return prop._PropData;
    }

}