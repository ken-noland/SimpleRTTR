namespace SimpleRTTR
{
    inline PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop);

    PropertyData::PropertyData(const stdrttr::string& name, const TypeReference& type, std::size_t offset)
        :
        _Name(name),
        _Type(type),
        _Offset(offset)
    {
    }

    PropertyData::PropertyData(PropertyData&& data)
        :
        _Name(std::move(data._Name)),
        _Type(data._Type),
        _Offset(data._Offset),
        _Flags(data._Flags),
        _Meta(std::move(data._Meta))
    {
    }

    PropertyData::PropertyData(const PropertyData& data)
        :
        _Name(data._Name),
        _Type(data._Type),
        _Offset(data._Offset),
        _Flags(data._Flags),
        _Meta(data._Meta)
    {

    }

    bool PropertyData::operator==(const PropertyData& data) const
    {
        return Equals(data);
    }

    bool PropertyData::Equals(const PropertyData& data) const
    {
        return (_Name.compare(data._Name) == 0) && _Offset == data._Offset;
    }

    const stdrttr::string& PropertyData::Name() const
    {
        return _Name;
    }

    Type PropertyData::Type() const
    {
        return _Type.Type();
    }

    const std::size_t PropertyData::Offset() const
    {
        return _Offset;
    }

    const PropertyData::MetaList& PropertyData::Meta() const
    {
        return _Meta;
    }

    inline PropertyData::MetaList& _InternalPropertyDataGetMetaListRef(PropertyData& prop)
    {
        return prop._Meta;
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

    bool Property::operator==(const Property& data) const
    {
        return Equals(data);
    }

    bool Property::Equals(const Property& data) const
    {
        return _PropData.Equals(data._PropData);
    }

    const stdrttr::string& Property::Name() const
    {
        return _PropData.Name();
    }

    const std::size_t Property::Offset() const
    {
        return _PropData.Offset();
    }

    const class Type Property::Type() const
    {
        return _PropData.Type();
    }

    const Property::MetaList& Property::Meta() const
    {
        return _PropData.Meta();
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

    inline PropertyData& _InternalPropertyGetPropertyDataRef(Property& prop)
    {
        return prop._PropData;
    }
}